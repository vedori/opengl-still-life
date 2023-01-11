#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "types.h"
#include "shader.h"

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 globalPosition);

    void draw() const;
    const glm::vec3 GlobalPosition{};
private:
    GLuint _vertexArrayObject{};
    uint32_t _elementCount {0};
};
