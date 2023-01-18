#pragma once
#include <glm/glm.hpp>

#include <iostream>

struct Vertex {
    glm::vec3 Position {0.f, 0.f, 0.f};
    glm::vec3 Color {1.f, 1.f, 1.f};
    glm::vec3 Normal {0.f, 0.f, 0.f};
    glm::vec2 Uv {1.f, 1.f};
};

struct GlobalPosition {
    glm::vec3 Scale {1.0f, 1.0f, 1.0f};
    glm::vec3 Translation {0.0f, 0.0f, 0.0f};
    glm::vec3 RotationAxis {0.0f, 1.0f, 0.0f};
    float RotationAngle = {0.0f};
};

struct Shapes {
    static inline std::vector<Vertex> triangleVertices = {
            {
                    .Position = {-1.0f, 1.0f, 0.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            },
            {
                    .Position = {-1.0f, 0.0f, 0.0f},
                    .Color = {0.0f, 1.0f, 0.0f}
            },
            {
                    .Color = {0.0f, 0.0f, 1.0f}
            },
            {
                    .Position = {1.0f, -1.0f, 0.0f},
                    .Color = {0.0f, 1.0f, 0.0f}
            },
            {
                    .Position = {1.0f, 0.0f, 0.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            }
    };

    static inline std::vector<uint32_t> triangleElements = {
            0, 1, 2,   // first triangle
            2, 3, 4    // second triangle
    };

    static inline std::vector<Vertex> pyramidVertices {};
    static inline std::vector<uint32_t> pyramidElements {};

    static void setPyramidSides (int sides) {
        // the distance from the base to the tip
        float depth = 1.0f;

        // how many sides / how round it will be
        pyramidElements.reserve(sides * 3);
        pyramidVertices.reserve(sides + 1);
        pyramidVertices = {
                // the tip of the cylinder
                {
                        .Position = {0.0f, 0.0f, depth},
                },
                // the center of the cylinder's base
                {
                        .Position = {0.0f, 0.0f, 0},
                },

                // ... the rest of the vertices will be generated
        };

        // generate all the vertices for the base's circumference given the number of sides
        for (int i = 2; i < sides + 2; i++) {

            float angle = glm::radians((360.0f / static_cast<float>(sides)) * static_cast<float>(i - 1));

            // vertices for the base of the pyramid
            pyramidVertices.emplace_back(Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 0.0f},
                    .Color = {glm::abs(glm::cos(angle)), 0.0f, glm::abs(glm::sin(angle))},
            });

            // draw the elements for the pyramid using the index of the specified vertices

            // the index of the center of the circle
            pyramidElements.emplace_back(0);

            // the index of the current vertex
            pyramidElements.emplace_back(i);

            // the index of the next vertex
            int nextVertexIndex = (i < sides + 1) ? i + 1 : 2;
            pyramidElements.emplace_back(nextVertexIndex);

            // the elements for the bottom surface
            pyramidElements.emplace_back(1);
            pyramidElements.emplace_back(i);
            pyramidElements.emplace_back(nextVertexIndex);

        }

    }

    static inline std::vector<Vertex> cylinderVertices{};
    static inline std::vector<uint32_t> cylinderElements {};

    static void setPrismSides(int sides) {
        // the distance from the base to the top
        float depth = 1.0f;

        cylinderElements.reserve(sides * 3);
        cylinderVertices.reserve(sides + 1);
        cylinderVertices = {
                { // the center of the prism's base
                    .Position = {0.0f, 0.0f, 0.0f}
                },
                { // the center of the prism's top
                    .Position = {0.0f, 0.0f, depth}
                }
        };

        // generate all the vertices for the prism given the number of steps
        for (int i = 2; i < sides * 2 + 2; i++) {
            float degrees = (360.0f / static_cast<float>(sides)) * static_cast<float>(i - 2);
            float angle = glm::radians(degrees);

            // vertices for the base of the prism
            cylinderVertices.emplace_back( Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), 0.0f},
                    .Color = {glm::abs(glm::cos(angle)), 0.0f, glm::abs(glm::sin(angle))},
            });

            //vertices for the top of the prism
            cylinderVertices.emplace_back( Vertex {
                    .Position = {glm::cos(angle), glm::sin(angle), depth},
                    .Color = {glm::abs(glm::cos(angle)), 0.5f, glm::abs(glm::sin(angle))},
            });

            // draw the elements for the prism using the index of the specified vertices

            // the index of the next top vertex
            int nextVertexIndex = (i < sides * 2 + 1) ? i + 1 : 2;
            cylinderElements.emplace_back(nextVertexIndex);

            // the index of the current vertex
            cylinderElements.emplace_back(i);

            // the index of the next bottom vertex
            int nextBottomVertexIndex = (i < sides * 2 + 1) ? i + 2 : 3;
            cylinderElements.emplace_back(nextBottomVertexIndex);

            // the elements for the top and bottom surfaces
            if (i % 2 == 0) {
                cylinderElements.emplace_back(0);
                cylinderElements.emplace_back(i);
                int nextIndex = (i < sides * 4) ? i + 2 : 2;
                cylinderElements.emplace_back(nextIndex);
            } else {
                cylinderElements.emplace_back(1);
                cylinderElements.emplace_back(i);
                int nextIndex = (i < sides * 4 + 1) ? i + 2 : 3;
                cylinderElements.emplace_back(nextIndex);
            }
        }

    }
};