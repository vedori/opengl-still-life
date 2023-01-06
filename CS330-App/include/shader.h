//
// Created by phil on 1/4/2023.
//
#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader {
public:
    // the program ID
    unsigned int ID {};

public:
    // constructor reads and builds the shader
    shader(const char *vertexPath, const char *fragmentPath);

    // use/activate the shader
    void use() const;

    // utility functions to set values of uniform variables
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void compileVShader(unsigned int *vertex, const char *code, char *log);
    void compileFShader(unsigned int *fragmentLoc, const char *code, char *log);
    void linkProgram(char *log);
    const char *readShaderFile(const char *shaderPath);
};
