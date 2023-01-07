#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class shader {
public:
    // the program ID
    GLuint ID;

public:
    // constructor reads and builds the shader
    shader(const char *vertexPath, const char *fragmentPath);

    // use/activate the shader
    void use() const;

    // utility functions to set values of uniform variables
    void setBool(const char *name, bool value) const;
    void setInt(const char *name, int value) const;
    void setFloat(const char *name, float value) const;

private:
    // utility functions to compile and link shaders to the shaderProgram
    static std::string readShaderFile(const char *shaderPath);
    static void compileVShader(unsigned int *vertex, const char *code, char *log);
    static void compileFShader(unsigned int *fragmentLoc, const char *code, char *log);
    void linkProgram(char *log) const;
};
