#include <iostream>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "application.h"
#include "types.h"


Application::Application(std::string WindowTitle, int Width, int Height) : _applicationName{std::move( WindowTitle )}, _width{ Width }, _height{ Height } {}

void Application::Run()
{
	if (!openWindow()) {
		return;
	}

    // load the mesh and shader data
    load();

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
    glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));

    // this function is called when the application is resized
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        // resizes the window / scene
        glViewport(0, 0, width, height);

        // update the width and height attributes of the application class
        auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        if (width  && height) {
            app->_width = width;
            app->_height = height;
        }
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

    // enables the depth buffer
    glEnable(GL_DEPTH_TEST);

	return true;
}

void Application::load()
{
    // places four pyramids as a guideline for the scene
//    Shapes::setPyramidSides(4);
//    float localWidth = 20.0;
//    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements).GlobalPosition = {
//            .Scale{0.05f, 0.05f, 0.05f},
//            .Translation{localWidth, 0.0f, localWidth},
//            .RotationAxis {1.0f, 0.0f, 0.0f},
//            .RotationAngle = glm::radians(-90.0f)
//    };
//    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements).GlobalPosition = {
//            .Scale{0.05f, 0.05f, 0.05f},
//            .Translation{-localWidth, 0.0f, localWidth},
//            .RotationAxis {1.0f, 0.0f, 0.0f},
//            .RotationAngle = glm::radians(-90.0f)
//    };
//    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements).GlobalPosition = {
//            .Scale{0.05f, 0.05f, 0.05f},
//            .Translation{localWidth, 0.0f, -localWidth},
//            .RotationAxis {1.0f, 0.0f, 0.0f},
//            .RotationAngle = glm::radians(-90.0f)
//    };
//    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements).GlobalPosition = {
//            .Scale{0.05f, 0.05f, 0.05f},
//            .Translation{-localWidth, 0.0f, -localWidth},
//            .RotationAxis {1.0f, 0.0f, 0.0f},
//            .RotationAngle = glm::radians(-90.0f)
//    };

    // loads data for the objects in the scene
    Shapes::setPrismSides(20);
    Shapes::setPyramidSides(20);
    float penRadius = 0.03f;
    float penRatio = 26.65f;

    // create a pen with a pyramid and a cylinder
    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements).GlobalPosition = {
            .Scale{penRadius, penRadius, penRadius * 2},
            .Translation{-19.5f, 0.0f, 2.02f},
            .RotationAxis {1.0f, 0.0f, 0.0f},
            .RotationAngle = glm::radians(180.0f)
    };
    _meshes.emplace_back(Shapes::cylinderVertices, Shapes::cylinderElements).GlobalPosition = {
            .Scale{penRadius, penRadius, penRadius * penRatio},
            .Translation{-19.5f, 0.0f, 0.15f}
    };

    // loads shaders
    _shader = Shader{"shaders/basic_shader.vert", "shaders/basic_shader.frag"};
}

void Application::draw() {
    // rendering the background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draws the first object in wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // apply matrix transformations and shaders to all the meshes
    for (auto& mesh : _meshes) {
        // model transforms the objects and sets their positions relative to the origin
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, mesh.GlobalPosition.Scale);
        model = glm::translate(model, mesh.GlobalPosition.Translation);
        model = glm::rotate(model, mesh.GlobalPosition.RotationAngle,  mesh.GlobalPosition.RotationAxis);

        // a lower view
//        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f),
//                                     glm::vec3(0.0f, 0.0f, 0.0f),
//                                     glm::vec3(0.0f, 1.0f, 0.0f));
        // a higher view
//        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f),
//                                     glm::vec3(0.0f, 0.0f, 0.0f),
//                                     glm::vec3(0.0f, 1.0f, 0.0f));
        // a low view
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));

        // rotates the view
//        view = glm::rotate(view, static_cast<float>(glfwGetTime()) / -10.5f ,glm::vec3(0.0f,1.0f,0.0f));

        // the projection view sets the fov, the aspect ratio and clipping range of the viewing box
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                      static_cast<float>(_width) / static_cast<float>(_height),
                                      0.1f, 100.0f);

        // sends the matrices to the shader
        _shader.SetMat4("model", model);
        _shader.SetMat4("view", view);
        _shader.SetMat4("projection", projection);
        _shader.Activate();

        mesh.Draw();

        // draws subsequent objects in fill mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

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