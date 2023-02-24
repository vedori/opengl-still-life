#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include "glm/detail/type_mat4x4.hpp"

class Shader {
public:
    // the program ID
    GLuint ID;

public:
    Shader() = default;
    // constructor reads and builds the shader from path
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    // use/activate the shader
    void Bind() const;

    // utility functions to set values of uniform variables
    void SetBool(const char *name, bool value) const;
    void SetInt(const char *name, int value) const;
    void SetFloat(const char *name, float value) const;
    void SetVec2(const char* name, const glm::vec2& value) const;
    void SetVec3(const char* name, const glm::vec3& value) const;
    void SetMat4(const char *name, const glm::mat4& value) const;

private:
    // utility functions to compile and link shaders to the shader program
    static std::string readShaderFile(const std::filesystem::path& shaderPath);
    static void compileVShader(GLuint& vertexShaderID, const char *code, char *log);
    static void compileFShader(GLuint& fragmentShaderID, const char *code, char *log);
    void linkProgram(char *log) const;

    // the size of the log buffer
    static const size_t LOG_SIZE = 512;
};
