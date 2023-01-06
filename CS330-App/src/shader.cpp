//
// Created by phil on 1/4/2023.
//
#include "shader.h"

shader::shader(const char* vertexPath, const char* fragmentPath) {
//    const char* vShaderCode = readShaderFile(vertexPath);
//    const char* fShaderCode = readShaderFile(fragmentPath);
    const char* vShaderCode = R"(#version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;

    out vec3 vertexRGB;

    void main ()
    {
        gl_Position = vec4(position, 1.0);
        vertexRGB = color;
    }
    )";
    const char* fShaderCode = R"(#version 330 core
    out vec4 FragColor;
    in vec3 vertexRGB;

    void main()
    {
        FragColor = vec4(vertexRGB, 1.0);
    })";

    // char buffer for logging
    char infoLog[512];

    // shader program
    ID = glCreateProgram();

    // compile shaders
    unsigned int vertex, fragment;
    compileVShader(&vertex, vShaderCode, infoLog);
    compileFShader(&fragment, fShaderCode, infoLog);

    linkProgram(infoLog);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

const char* shader::readShaderFile(const char* shaderPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string code;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open file
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        code = shaderStream.str();
    }
    catch(std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    return code.c_str();
}

void shader::compileVShader(unsigned int* vertexLoc, const char* code, char *log) {
    // TODO: delete the linked shaders
    *vertexLoc = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(*vertexLoc, 1, &code, nullptr);
    glCompileShader(*vertexLoc);
    // print compile errors if any
    int success;
    glGetShaderiv(*vertexLoc, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(*vertexLoc, 512, nullptr, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
    };
    glAttachShader(ID, *vertexLoc);
//    glDeleteShader(*vertexLoc);
}

void shader::compileFShader(unsigned int* fragmentLoc, const char* code, char *log) {
    // fragment Shader
    *fragmentLoc = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(*fragmentLoc, 1, &code, nullptr);
    glCompileShader(*fragmentLoc);
    // print compile errors if any
    int success;
    glGetShaderiv(*fragmentLoc, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(*fragmentLoc, 512, nullptr, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
    };
    glAttachShader(ID, *fragmentLoc);
}

void shader::linkProgram(char *log) {
    glLinkProgram(ID);

    //print linking errors if any
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}

void shader::use() const {
    glUseProgram(ID);
}

void shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
