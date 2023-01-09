#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "types.h"
#include "shader.h"

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);

    void draw();
private:
    GLuint _vertexArrayObject{};
    uint32_t _elementCount {0};
    Shader _shader{};
};
