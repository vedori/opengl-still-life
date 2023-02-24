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

    // represents the vertices of an OpenGL object
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, Texture texture);

    // draws the vertices to the scene
    void Draw() const;

    // gets the texture's shader
    const Shader& GetShader();

    // the material properties of the mesh for lighting
    Material Material;

    GlobalPosition GlobalPosition;
protected:
    Texture _texture;
    GLuint _vertexArrayObject{};
    uint32_t _elementCount {0};
};
