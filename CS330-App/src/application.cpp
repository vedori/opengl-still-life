#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "application.h"
#include "types.h"

// defines a macro to set the first texture to the default one


Application::Application(std::string WindowTitle, int Width, int Height) : _applicationName{std::move( WindowTitle )}, _width{ Width }, _height{ Height } {}

void Application::Run()
{
	if (!openWindow()) {
		return;
	}

    // for FPS counting
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // loads data relevant to the scene
    load();

    // render loop
	while (true) {
		if (glfwWindowShouldClose(_window)) {
			break;
		}

        // Measure speed (60 fps => 16.6666ms, 30 fps => 33.33333ms
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last printf() was more than 1 sec ago
            // printf and reset timer
//            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
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

    // enables the program to use this window
	glfwMakeContextCurrent(_window);

    // hides the mouse cursor and centers it to the screen
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // enables the program to use this window in callbacks
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
    // loads shaders
    _shader = Shader{"shaders/basic_shader.vert", "shaders/basic_shader.frag"};

    // config textures
    const auto brickTexture = Texture(_shader, "textures/bricks.jpg");
    const auto plasticTexture =  Texture(_shader, "textures/plastic.jpg");
    const auto bandannaTexture =  Texture(_shader, "textures/bandanna.jpg");

    // Loads data for the objects in the scene ...

    // create a pen with a pyramid and a cylinder and a texture tiling of 5
    Shapes::setPrismSides(44, {5.0f,5.0f});
    Shapes::setPyramidSides(44, {5.0f, 5.0f});
    const float PEN_RADIUS = 0.3f;
    const float PEN_RADIUS_LENGTH_RATIO = 26.65f;
    const float PEN_X = -19.5f;
    const float PEN_Y = 1.0f;
    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements, plasticTexture)
    .GlobalPosition = {
            .Scale{PEN_RADIUS, PEN_RADIUS, PEN_RADIUS * 2},
            .Translation{PEN_X, PEN_Y, 1.35f},
            .RotationAxis {1.0f, 0.0f, 0.0f},
            .RotationAngle = glm::radians(180.0f)
    };
    _meshes.emplace_back(Shapes::prismVertices, Shapes::prismElements, plasticTexture)
    .GlobalPosition = {
            .Scale{PEN_RADIUS, PEN_RADIUS, PEN_RADIUS * PEN_RADIUS_LENGTH_RATIO},
            .Translation{PEN_X, PEN_Y, 0.1f}
    };


     // places a plane for our scene
//     Shapes::setPlaneTiling(2.0f);
    _meshes.emplace_back(Shapes::planeVertices, Shapes::planeElements, bandannaTexture )
    .GlobalPosition = {
            .Scale{30.5f, 30.5f, 30.5f},
            .Translation{-0.5f, -0.001f, -0.5f}
    };

    // test pyramid with texture tiling
    auto testTile = 4.0f;
    Shapes::setPyramidSides(4, {testTile, testTile});
    _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements, brickTexture)
    .GlobalPosition = {
            .Scale{2.0f, 4.0f, 2.0f},
            .Translation{0.0f, 0.5f, -4.0f},
            .RotationAxis{glm::vec3(1.0f,0.0f,0.0f)},
            .RotationAngle = glm::radians(-90.0f),
    };

    // sets up the camera's position
    _camera = Camera(glm::vec3(0.0f, 4.0f, 1.0f));
//    _camera = Camera(glm::vec3(0.0f, 20.5f, 1.0f), glm::vec3(0.0f, -24.0f, -1.0f));
}

void Application::draw() {
    // rendering the background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // apply matrix transformations and shaders to all the meshes
    for (auto& mesh : _meshes) {
        // model transforms the objects and sets their positions relative to the origin
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, mesh.GlobalPosition.Scale);
        model = glm::translate(model, mesh.GlobalPosition.Translation);
        model = glm::rotate(model, mesh.GlobalPosition.RotationAngle,  mesh.GlobalPosition.RotationAxis);

        // sends the matrices to the shader
        _shader.SetMat4("model", model);
        _shader.SetMat4("view", _camera.View());
        _shader.SetMat4("projection", Camera::Projection());
        _shader.Activate();

        mesh.Draw();

    }

    // check and call events and swap the buffers
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Application::input()
{
    // ends the program if the Q key is pressed
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
	}

    // Strafe the camera view when holding W,S,A,D keys
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::FORWARD);
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::BACKWARD);
    }
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::LEFT);
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::RIGHT);
    }

    // Hold Q/E to move up or down
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::UP);
    }
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) {
        _camera.Move(Camera::Movement::DOWN);
    }

    // sets the last frame to the time this function runs in the render loop to standardize movement
    Camera::CaptureFrame();
}