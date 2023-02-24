#include "texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

// activates up to 16 textures, some CPUs require this before texture binding
//void Texture::ActivateTextures() {
//    for (int i = 0; i < TextureIds.size(); i++) {
//        if (i > 15)
//            break;
//        glActiveTexture(GL_TEXTURE0 + i);
//    }
//}

// binds the texture to the shader
void Texture::Bind() const {
    // binds the specified texture to the object via its ID
    // subsequent texture configs point to this ID
    glBindTexture(GL_TEXTURE_2D, TextureId);
    _shader.SetVec2("uvScale", _uvScale);
}

Texture::Texture(const Shader& shader, const std::string& filePath, const glm::vec2& uvScale) : _shader{shader}, _uvScale{uvScale} {
    // sets up and binds the texture to OpenGL
    glGenTextures(1, &TextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    // set the texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set the texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // set the texture minimap filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // loads texture image data
    int width, height, nChannels;
    auto data = stbi_load(filePath.c_str(), &width, &height, &nChannels, 0);
    if (data) {
        // uses alpha channel if the data supports it
        if (nChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if (nChannels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            std::cout << "Cannot handle image with " << nChannels << " channels" << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // frees the data and resets the active texture
    stbi_image_free(data);
    glActiveTexture(GL_TEXTURE0);
}

const Shader &Texture::GetShader() {
    return _shader;
}

