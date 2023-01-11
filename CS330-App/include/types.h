#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position {0.f, 0.f, 0.f};
    glm::vec3 Color {1.f, 1.f, 1.f};
    glm::vec3 Normal {0.f, 0.f, 0.f};
    glm::vec2 Uv {1.f, 1.f};
};

struct GlobalPosition {
    glm::vec3 Scale {1.0f, 1.0f, 1.0f};
    glm::vec3 Translation {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation {0.0f, 0.0f, 0.0f};
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

    static inline std::vector<Vertex> pyramidVertices = {
            // base
            {
                    .Position = {-1.0f, 0.0f, -1.0f},
                    .Color = {0.0f, 0.0f, 1.0f}
            },
            {
                    .Position = {-1.0f, 0.0f, 1.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            },
            {
                    .Position = {1.0f, 0.0f, 1.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            },
            {
                    .Position = {1.0f, 0.0f, -1.0f},
                    .Color = {0.0f, 0.0f, 1.0f}
            },
            // tip
            {
                    .Position = {0.0f, 2.0f, 0.0f},
                    .Color = {0.0f, 1.0f, 0.0f}
            },
    };

    static inline std::vector<uint32_t> pyramidElements = {
            // base
            0, 1, 3,
            3, 2, 1,

            1, 2, 4,    // front face
            0, 3, 4,    // back face
            0, 1, 4,    // left face
            2, 3, 4     // right face
    };
};