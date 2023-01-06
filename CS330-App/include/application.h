#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
//#include <shad

class Application {
public:
	Application(std::string WindowTitle, int width, int height);
	void Run();

private:
    bool openWindow();
    void initMesh();
    void draw();
    void input();

private:
	std::string _applicationName {};
	int _width {};
	int _height {};
	GLFWwindow* _window {nullptr};
    GLuint _vertexArrayObject{};
};