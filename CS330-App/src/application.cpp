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

    // shaders specify attributes for the mesh data
    shader rainbowShader{"shaders/shader.vs", "shaders/shader.fs"};

    // render loop
	while (true) {
		if (glfwWindowShouldClose(_window)) {
			break;
		}

        input();

        // use shaders
        rainbowShader.use();

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

    // bind the VAO and EBO for OpenGL to send mesh data to GPU
	// the VAO holds all subsequent VBOs
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);

    // create VBO for OpenGL to send vertex data to the VAO
	unsigned int vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    // create EBO for OpenGL to send index data to GPU to avoid overhead resulting in overlapping lines
    unsigned int elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    // OpenGL needs to know how to connect the vertex data to the shader attributes
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
}

void Application::draw() {
    // rendering the background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //drawing the scene
    glBindVertexArray(_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

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