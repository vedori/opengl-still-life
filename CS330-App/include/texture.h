#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"

class Texture {
public:
    Texture() = default;

    // constructs a texture using the shader and the file path of the texture image
    explicit Texture(const Shader& shader, const std::string& filePath = "assets/textures/default.jpg", const glm::vec2& uvScale = {1.0f,1.0f});

    // binds the texture to the shader
    void Bind() const;

    //    // activates up to 16 textures per texel, some CPUs require this before texture binding
    //    static void ActivateTextures();
public:
    // The OpenGL ID for the texture to be set by a call to glBindTexture when applied to a mesh
    GLuint TextureId{};

    // returns an immutable reference to the shader
    const Shader& GetShader();

private:
    // represents the shader program
    const Shader& _shader{};

    // the scale to apply to uv coordinates
    const glm::vec2 _uvScale{};
};