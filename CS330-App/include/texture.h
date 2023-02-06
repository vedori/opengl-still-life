#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"

class Texture {
public:
    Texture() = default;
    // needs the shader's id and the file path of the texture image
    explicit Texture(const Shader& shader, const std::string& filePath = "textures/default.jpg");

    GLuint TextureId{};

    // activates up to 16 textures, some CPUs require this before texture binding
//    static void ActivateTextures();

    // binds the texture to the shader
    void Bind() const;

private:
//    const GLuint _shaderId{};
    const Shader& _shader{};
};

