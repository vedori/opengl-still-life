#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

struct Vertex {
    glm::vec3 Position {0.f, 0.f, 0.f};
    glm::vec3 Color {1.f, 1.f, 1.f};
    glm::vec2 Uv {1.0f, 1.0f};
    glm::vec3 Normal {0.f, 0.f, 0.f};
};

struct GlobalPosition {
    glm::vec3 Scale {1.0f, 1.0f, 1.0f};
    glm::vec3 Translation {0.0f, 0.0f, 0.0f};
    glm::mat4 Rotation {1.0f};
    glm::vec3 RotationAxis {0.0f, 1.0f, 0.0f};
    float RotationAngle = {0.0f};
};

struct Material {
    // configured with default values following the table
    // http://devernay.free.fr/cours/opengl/materials.html
    // for black plastic
    float Diffuse = 0.01f;
    float Specular = 0.5f;
    float Shininess = 0.25f;

    void SendToShader(const Shader& shader) const {
        shader.SetFloat("material.diffuse", Diffuse);
        shader.SetFloat("material.specular", Specular);
        shader.SetFloat("material.shininess", Shininess);
    }
};

struct DirectionalLight {
    glm::vec3 Direction;
    glm::vec3 Ambient = {1, 1, 1}; // the color of the ambient light
    glm::vec3 Diffuse = {1, 1, 1}; // the color of the diffuse light
    glm::vec3 Specular = glm::vec3{1.0f}; // the specular constant

    void SendToShader(const Shader& shader) const {
        shader.SetVec3("dirlight.direction", Direction);
        shader.SetVec3("dirlight.ambient", Ambient);
        shader.SetVec3("dirlight.diffuse", Diffuse);
        shader.SetVec3("dirlight.specular", Specular);
    }
};

struct PointLight {
    glm::vec3 Position;
    glm::vec3 Ambient = {0, 0, 0}; // the color of the ambient light
    glm::vec3 Diffuse = {1, 1, 1}; // the color of the diffuse light
    glm::vec3 Specular = glm::vec3{1.0f};

    // configured with default values for attenuation following the table
    // (http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation)
    // at a distance of 600
    float Constant = 1.0f;
    float Linear = 0.007f;
    float Quadratic = 0.0002f;

    void SendToShader(const Shader& shader) const {
        shader.SetVec3("pointlight.position", Position);
        shader.SetVec3("pointlight.ambient", Ambient);
        shader.SetVec3("pointlight.diffuse", Diffuse);
        shader.SetVec3("pointlight.specular", Specular);
        shader.SetFloat("pointlight.constant", Constant);
        shader.SetFloat("pointlight.linear", Linear);
        shader.SetFloat("pointlight.quadratic", Quadratic);
    }
};

struct Shapes {
    static inline void UpdateNormals(Vertex& v1, Vertex& v2, Vertex& v3) {
        glm::vec3 U = v2.Position - v1.Position;
        glm::vec3 V = v3.Position - v1.Position;

        auto normal = glm::cross(U, V);

        v1.Normal = normal;
        v2.Normal = normal;
        v3.Normal = normal;
    }

    static inline std::vector<Vertex> planeVertices = {
            { // bottom left corner
                    .Position = {0.0f, 0.0f, 0.0f},
                    .Uv = {0.0f, 0.0f},
            },
            { // top left corner
                    .Position = {0.0f, 0.0f, 1.0f},
                    .Uv = {0.0f, 1.0f},
            },
            { // bottom right corner
                    .Position = {1.0f, 0.0f, 0.0f},
                    .Uv = {1.0f, 0.0f},
            },
            { // top right corner
                    .Position = {1.0f, 0.0f, 1.0f},
                    .Uv = {1.0f, 1.0f},
            },
    };;

    static inline std::vector<uint32_t> planeElements = {
            0, 1, 3,
            0, 3, 2,
    };

    static inline std::vector<Vertex> pyramidVertices {};
    static inline std::vector<uint32_t> pyramidElements {};

    static void setPyramidSides (int sides) {
        // clears data from any previous calls to this function
        pyramidVertices.clear();
        pyramidElements.clear();

        pyramidVertices = {
                // the tip of the pyramid
                {
                        .Position = {0.0f, 0.0f, 1.0f},
                        .Uv = {0.5f, 0.5f},
                },
                // the center of the pyramid's base
                {
                        .Uv = {0.5f, 0.5f},
                },

                // ... the rest of the vertices will be generated
        };

        // generate all the vertices for the base's circumference given the number of sides
        for (int i = 2; i < sides + 2; i++) {
            auto degrees = (360.0f / static_cast<float>(sides)) * static_cast<float>(i - 2);
            float angle = glm::radians(degrees);

            // texture coordinate transformation
            glm::vec2 coordinates = {glm::cos(angle), glm::sin(angle)};
            float percentX = static_cast<float>(i - 2) / static_cast<float>(sides);
            float uvXPos = 2.0f * glm::abs(percentX - 0.5f);

            // vertices for the base of the pyramid
            pyramidVertices.emplace_back(Vertex{
                    .Position = {coordinates, 0.0f},
                    .Uv = {uvXPos, 0.0f},
            });

            // draw the elements for the pyramid using the index of the specified vertices
            int nextVertexIndex = (i < sides + 1) ? i + 1 : 2;

            pyramidElements.emplace_back(i);
            pyramidElements.emplace_back(1);
            pyramidElements.emplace_back(nextVertexIndex);

            pyramidElements.emplace_back(i);
            pyramidElements.emplace_back(nextVertexIndex);
            pyramidElements.emplace_back(0);
        }
    }

    static inline std::vector<Vertex> prismVertices{};
    static inline std::vector<uint32_t> prismElements {};

    static void setPrismSides(int sides) {
        // clears data from any previous calls to this function
        prismVertices.clear();
        prismElements.clear();

        prismVertices = {
                { // the center of the prism's base
                    .Position = {0.0f, 0.0f, 0.0f},
                    .Uv = {0.5f, 0.5f},
                },
                { // the center of the prism's top
                    .Position = {0.0f, 0.0f, 1.0f},
                    .Uv = {0.5f, 0.5f},
                }
        };

        // generate all the vertices for the prism given the number of steps
        for (int i = 2; i < sides * 2 + 2; i++) {
            float degrees = (360.0f / static_cast<float>(sides)) * static_cast<float>(i - 2);
            float angle = glm::radians(degrees);

            // texture coordinate transformation
            float uvXPos = ((i - 2) < sides) ?
                    static_cast<float>(i - 2) / static_cast<float>(sides) :
                    static_cast<float>(i - sides - 2) / static_cast<float>(sides);

            // normals for each vertex
            glm::vec3 normal = {glm::cos(angle), 0.0f, glm::sin(angle)};

            // vertices for the base of the prism
            prismVertices.emplace_back(Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 0.0f},
                    .Uv = {uvXPos, 0.0f},
            });

            //vertices for the top of the prism
            prismVertices.emplace_back(Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 1.0f},
                    .Uv = {uvXPos, 1.0f},
            });

            // draws the elements for the prism using the index of the specified vertices ...

            // the index of the next top vertex
            int nextVertexIndex = (i < sides * 2 + 1) ? i + 1 : 2;

            // the index of the next bottom vertex
            int nextBottomVertexIndex = (i < sides * 2 + 1) ? i + 2 : 3;

            prismElements.emplace_back(nextVertexIndex);
            prismElements.emplace_back(nextBottomVertexIndex);
            prismElements.emplace_back(i);

            prismElements.emplace_back(i);
            prismElements.emplace_back(nextBottomVertexIndex);
            prismElements.emplace_back(nextVertexIndex);

            // the elements for the top and bottom surfaces
            if (i % 2 == 0) {
                int nextIndex = (i < sides * 4) ? i + 2 : 2;
                prismElements.emplace_back(nextIndex);
                prismElements.emplace_back(0);
                prismElements.emplace_back(i);
            }
            else {
                int nextIndex = (i < sides * 4 + 1) ? i + 2 : 3;
                prismElements.emplace_back(i);
                prismElements.emplace_back(nextIndex);
                prismElements.emplace_back(1);
            }
        }
    }
};