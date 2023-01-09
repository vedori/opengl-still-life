#include <iostream>
#include <utility>
#include <vector>
#include "application.h"
#include "types.h"

Application::Application(std::string WindowTitle, int Width, int Height) : _applicationName{std::move( WindowTitle )}, _width{ Width }, _height{ Height } {}

void Application::Run()
{
	if (!openWindow()) {
		return;
	}

    // load the mesh data
    initMesh();

    // render loop
	while (true) {
		if (glfwWindowShouldClose(_window)) {
			break;
		}

        // handle input
        input();

        // draw the scene
        draw();
	}
	glfwTerminate();
}

bool Application::openWindow()
{
	// initializes the GLFW Library to support version >= 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, _applicationName.c_str(), nullptr, nullptr);

	// terminates the program if the window, OpenGL context, and GLAD functions are not loaded correctly
	if (!_window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);

    // this function is called when the application is resized
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		// resizes the window / scene
		glViewport(0, 0, width, height);
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

void Application::initMesh()
{
    std::vector<Vertex> vertices = {
            {
                    .Position = {-1.0f, 1.0f, 0.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            },
            {
                    .Position = {-1.0f, 0.0f, 0.0f},
                    .Color = {0.0f, 1.0f, 0.0f}
            },
            {
                    .Color = {0.0f, 0.0f, 1.0f}
            },
            {
                    .Position = {1.0f, 0.0f, 0.0f},
                    .Color = {1.0f, 0.0f, 0.0f}
            },
            {
                    .Position = {1.0f, -1.0f, 0.0f},
                    .Color = {0.0f, 1.0f, 0.0f}
            }
    };

	std::vector<uint32_t> indices = {
		0, 1, 2,   // first triangle
		2, 3, 4    // second triangle
	};

    // creates two right triangles that share a corner
    _mesh = Mesh{std::move(vertices), std::move(indices)};
}

void Application::draw() {
    // rendering the background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //drawing the scene
    _mesh.draw();

    // check and call events and swap the buffers
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Application::input()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
	}
	
}