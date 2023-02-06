#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <algorithm>
#include <iostream>

struct Vertex {
    glm::vec3 Position {0.f, 0.f, 0.f};
    glm::vec3 Color {1.f, 1.f, 1.f};
    glm::vec3 Normal {0.f, 0.f, 0.f};
    glm::vec2 Uv {1.0f, 1.0f};
};

struct GlobalPosition {
    glm::vec3 Scale {1.0f, 1.0f, 1.0f};
    glm::vec3 Translation {0.0f, 0.0f, 0.0f};
    glm::vec3 RotationAxis {0.0f, 1.0f, 0.0f};
    float RotationAngle = {0.0f};
};

struct Shapes {

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
            0, 1, 2,
            2, 3, 1
    };

    // sets the number of divisions in the texture tiling
    static void setPlaneTiling(float divisions) {
        planeVertices.clear();
        planeVertices = {
                { // bottom left corner
                        .Position = {0.0f, 0.0f, 0.0f},
                        .Uv = {0.0f, 0.0f},
                },
                { // top left corner
                        .Position = {0.0f, 0.0f, 1.0f},
                        .Uv = {0.0f, divisions},
                },
                { // bottom right corner
                        .Position = {1.0f, 0.0f, 0.0f},
                        .Uv = {divisions, 0.0f},
                },
                { // top right corner
                        .Position = {1.0f, 0.0f, 1.0f},
                        .Uv = {divisions, divisions},
                },
        };
    }

    static inline std::vector<Vertex> pyramidVertices {};
    static inline std::vector<uint32_t> pyramidElements {};

    static void setPyramidSides (int sides, const glm::vec2& uvScale = {1.0f, 1.0f}) {
        // clears data from any previous calls to this function
        pyramidVertices.clear();
        pyramidElements.clear();

        pyramidVertices = {
                // the tip of the pyramid
                {
                        .Position = {0.0f, 0.0f, 1.0f},
                        .Uv = glm::vec2(1.0f, 1.0f) * uvScale,
                },
                // the center of the pyramid's base
                {
                        .Uv = glm::vec2(1.0f, 1.0f) * uvScale,
                },

                // ... the rest of the vertices will be generated
        };

        // generate all the vertices for the base's circumference given the number of sides
        for (int i = 2; i < sides + 2; i++) {
            auto degrees = (360.0f / static_cast<float>(sides)) * static_cast<float>(i - 2);
            float angle = glm::radians(degrees);

            // texture coordinate transformation
            glm::vec2 coordinates = {glm::cos(angle), glm::sin(angle)};
            float uvXPos = glm::pi<float>() / 5.0f * glm::abs(angle - glm::half_pi<float>());

            // vertices for the base of the pyramid
            pyramidVertices.emplace_back(Vertex {
                    .Position = {coordinates, 0.0f},
                    .Uv = glm::vec2(uvXPos, 0.0f) * uvScale,
            });

            // draw the elements for the pyramid using the index of the specified vertices
            // the index of tip of the pyramid
            pyramidElements.emplace_back(0);

            // the index of the current vertex
            pyramidElements.emplace_back(i);

            // the index of the next vertex
            int nextVertexIndex = (i < sides + 1) ? i + 1 : 2;
            pyramidElements.emplace_back(nextVertexIndex);

            // the index for the bottom surface
            pyramidElements.emplace_back(1);
            pyramidElements.emplace_back(i);
            pyramidElements.emplace_back(nextVertexIndex);
        }

        pyramidVertices.emplace_back(Vertex {
                .Position = {1.0f, 1.0f, 0.0f}
        });
    }

    static inline std::vector<Vertex> prismVertices{};
    static inline std::vector<uint32_t> prismElements {};

    static void setPrismSides(int sides, const glm::vec2& uvScale = {1.0f, 1.0f}) {
        // clears data from any previous calls to this function
        prismVertices.clear();
        prismElements.clear();

        prismVertices = {
                { // the center of the prism's base
                    .Position = {0.0f, 0.0f, 0.0f},
                    .Uv = glm::vec2(0.5f, 0.5f) * uvScale,
                },
                { // the center of the prism's top
                    .Position = {0.0f, 0.0f, 1.0f},
                    .Uv = glm::vec2(0.5f, 0.5f) * uvScale,
                }
        };

        // generate all the vertices for the prism given the number of steps
        for (int i = 2; i < sides * 2 + 2; i++) {
            float degrees = (360.0f / static_cast<float>(sides)) * static_cast<float>(i - 2);
            float angle = glm::radians(degrees);

            // texture coordinate transformation
            float uvXPos = static_cast<float>(i - 2) / static_cast<float>(sides);

            // vertices for the base of the prism
            prismVertices.emplace_back(Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 0.0f},
                    .Uv = glm::vec2(uvXPos, 0.0f) * uvScale,
            });

            //vertices for the top of the prism
            prismVertices.emplace_back(Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 1.0f},
                    .Uv = glm::vec2(uvXPos, 1.0f) * uvScale,
            });

            // draw the elements for the prism using the index of the specified vertices

            // the index of the next top vertex
            int nextVertexIndex = (i < sides * 2 + 1) ? i + 1 : 2;
            prismElements.emplace_back(nextVertexIndex);

            // the index of the current vertex
            prismElements.emplace_back(i);

            // the index of the next bottom vertex
            int nextBottomVertexIndex = (i < sides * 2 + 1) ? i + 2 : 3;
            prismElements.emplace_back(nextBottomVertexIndex);

            // the elements for the top and bottom surfaces
            if (i % 2 == 0) {
                prismElements.emplace_back(0);
                prismElements.emplace_back(i);
                int nextIndex = (i < sides * 4) ? i + 2 : 2;
                prismElements.emplace_back(nextIndex);
            }
            else {
                prismElements.emplace_back(1);
                prismElements.emplace_back(i);
                int nextIndex = (i < sides * 4 + 1) ? i + 2 : 3;
                prismElements.emplace_back(nextIndex);
            }
        }
//        prismVertices.emplace_back(Vertex {
//                .Position = {0.0f, 0.0f, 1.0f},
//                .Uv = {0.5f, 0.5f},
//        });

    }
};