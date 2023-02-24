#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "mesh.h"
#include "camera.h"

class Application {
public:
	Application(std::string WindowTitle, int width, int height);
	void Run();

private:
    bool openWindow();
    void load();
    void draw();
    void input();

private:
	std::string _applicationName {};
	int _width {};
	int _height {};
	GLFWwindow* _window {nullptr};
    std::vector<Mesh> _meshes {};
    std::vector<Texture> _textures {};
    Shader _shader{};
    Shader _lampShader{};
    DirectionalLight _dLight;
    PointLight _pLight;
    Camera _camera{};
};