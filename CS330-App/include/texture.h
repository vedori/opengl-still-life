#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"

class Texture {
public:
    Texture() = default;

    // constructs a texture using the shader's id and the file path of the texture image
    explicit Texture(const Shader& shader, const std::string& filePath = "textures/default.jpg", const glm::vec2& uvScale = {1.0f,1.0f});

    // The OpenGL ID for the texture
    GLuint TextureId{};

    // The scale to apply to uv coordinates
    glm::vec2 UvScale{};

//    // activates up to 16 textures, some CPUs require this before texture binding
//    static void ActivateTextures();

    // binds the texture to the shader
    void Bind() const;

private:
//    const GLuint _shaderId{};
    const Shader& _shader{};
};

