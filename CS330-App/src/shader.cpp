//
// Created by phil on 1/4/2023.
//
#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vShaderCode = readShaderFile(vertexPath);
    std::string fShaderCode = readShaderFile(fragmentPath);

    // char buffer for logging
    char infoLog[_logSize];

    // shader program
    ID = glCreateProgram();

    // compile shaders
    GLuint vertexShaderID, fragmentShaderID = 0;
    compileVShader(vertexShaderID, vShaderCode.c_str(), infoLog);
    compileFShader(fragmentShaderID, fShaderCode.c_str(), infoLog);

    // attach shaders
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);

    linkProgram(infoLog);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

std::string Shader::readShaderFile(const char* shaderPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string code;
    std::ifstream shaderFile;

    // ensure that the input file stream object can throw exceptions:
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
    return code;
}

void Shader::compileVShader(GLuint& vertexShaderID, const char* code, char *log) {
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &code, nullptr);
    glCompileShader(vertexShaderID);
    // print compile errors if any
    int success;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShaderID, _logSize, nullptr, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
    }
}

void Shader::compileFShader(GLuint& fragmentShaderID, const char* code, char *log)  {
    // fragment Shader
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &code, nullptr);
    glCompileShader(fragmentShaderID);
    // print compile errors if any
    int success;
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShaderID, _logSize, nullptr, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
    }
}

void Shader::linkProgram(char *log) const {
    glLinkProgram(ID);

    //print linking errors if any
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, _logSize, nullptr, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
}