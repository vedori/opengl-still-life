#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "types.h"
#include "shader.h"
#include "texture.h"

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, const Texture&);
//    void SetTexture(Texture texture);
    void Draw() const;
    GlobalPosition GlobalPosition;
private:
    const Texture _texture;
    GLuint _vertexArrayObject{};
    uint32_t _elementCount {0};
};
