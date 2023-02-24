#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "application.h"
#include "types.h"
#include <ranges>


Application::Application(std::string WindowTitle, int Width, int Height) : _applicationName{std::move( WindowTitle )}, _width{ Width }, _height{ Height } {}

void Application::Run()
{
	if (!openWindow()) {
		return;
	}

    // loads data relevant to the scene
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

//    // DEBUG: enables back face culling
//    glFrontFace(GL_CCW);
//    glCullFace(GL_BACK);
//    glEnable(GL_CULL_FACE);

	return true;
}

void Application::load()
{
    // loads shaders
    _shader = Shader{"assets/shaders/basic_shader.vert", "assets/shaders/basic_shader.frag"};
    _lampShader = {"assets/shaders/light_shader.vert", "assets/shaders/light_shader.frag"};

    // config textures
    _textures.emplace_back(_shader, "assets/textures/plastic.jpg");
    _textures.emplace_back(_shader, "assets/textures/plastic.jpg", glm::vec2{1.0f, 5.0f});
    _textures.emplace_back(_shader, "assets/textures/bandanna.jpg");
    _textures.emplace_back(_shader, "assets/textures/blue-plastic.jpg", glm::vec2{10.0f, 1.0f});
    _textures.emplace_back(_shader, "assets/textures/paper.png");

    // used to iterate over the textures
    auto currentTexture = _textures.begin();

    // returns the current texture and sets the iterator to the next texture, or to the first texture if is at the end
    auto nextTexture = [currentTexture, this]() mutable {
        auto current = *currentTexture++;
        if (currentTexture == _textures.end()) {
            currentTexture = _textures.begin();
        }
        return current;
    };

    // Loads data for the objects in the scene ...

    // create a pen with a pyramid and a cylinder
    Shapes::setPyramidSides(44);
    Shapes::setPrismSides(44);
    const float PEN_RADIUS = 0.3f;
    const float PEN_RADIUS_LENGTH_RATIO = 26.65f;
    const float PEN_X = 10.0f;
    const float PEN_Y = PEN_RADIUS;
    const float PEN_Z = 15.95f;

    // pen tip
    Mesh penTip = {Shapes::pyramidVertices, Shapes::pyramidElements, nextTexture()};
    penTip.GlobalPosition = {
            .Scale{PEN_RADIUS, PEN_RADIUS, PEN_RADIUS * 2},
            .Translation{PEN_X, PEN_Y, PEN_Z - 0.0001f},
            .RotationAxis {1.0f, 0.0f, 0.0f},
            .RotationAngle = glm::radians(180.0f)
    };
    penTip.Material = {
            .Diffuse = 0.1f,
            .Shininess = 128.0f
    };

    // pen
    Mesh pen = {Shapes::prismVertices, Shapes::prismElements, nextTexture()};
    pen.GlobalPosition = {
            .Scale {PEN_RADIUS, PEN_RADIUS, PEN_RADIUS * PEN_RADIUS_LENGTH_RATIO},
            .Translation {PEN_X, PEN_Y, PEN_Z},
            .RotationAxis {0.0f, 0.0f, 1.0f},
            .RotationAngle = glm::radians(45.0f)
//            .RotationAngle = glm::radians(-90.0f)
    };
    pen.Material = {
            .Diffuse = 0.07f,
            .Specular = 0.1f,
            .Shininess = 256.0f
    };

    // places a bandanna plane for our scene
    Mesh plane = {Shapes::planeVertices, Shapes::planeElements, nextTexture()};
    plane.GlobalPosition = {
            .Scale {30.5f, 30.5f, 30.5f},
            .Translation {-0.5f, -0.001f, 0.0f}
    };
    plane.Material = {
            .Diffuse = 0.4f,
            .Specular = 1.0f,
            .Shininess = 32.0f
    };

    // adds the pencil eraser
    Shapes::setPrismSides(44);
    Mesh eraser = {Shapes::prismVertices, Shapes::prismElements, nextTexture()};
    glm::mat4 eraserRotation = glm::mat4(1.0f);
    eraserRotation = glm::rotate(eraserRotation, glm::radians(90.0f), {1.0f, 0.0f, 0.0f});
    eraserRotation = glm::rotate(eraserRotation, glm::radians(98.1f), {0.0f, 0.0f, 1.0f});
    eraser.GlobalPosition = {
            .Scale {1.0f, 0.8f, 1.0f},
            .Translation {8.0f, 1.0f, 11.0f},
            .Rotation = eraserRotation
    };
    eraser.Material = {
            .Diffuse = 1.0f,
            .Specular = 1.0f,
            .Shininess = 256.0f
    };

    // adds the notebook
    Shapes::setPrismSides(4);
    Mesh notebook = {Shapes::prismVertices, Shapes::prismElements, nextTexture()};
    glm::mat4 notebookRotation = glm::mat4(1.0f);
    notebookRotation = glm::rotate(notebookRotation, glm::radians(135.0f), {0, 0, 1});
    notebook.GlobalPosition = {
            .Scale {7.0f, 0.5f, 13.0f},
            .Translation {20.0f, 0.355f, 10.0f},
            .Rotation = notebookRotation,
    };
    notebook.Material = {
            .Diffuse = 0.0f,
            .Specular = 0.9f,
            .Shininess = 0.8f
    };

    // adds meshes to the mesh vector
    _meshes.emplace_back(pen);
    _meshes.emplace_back(penTip);
    _meshes.emplace_back(plane);
    _meshes.emplace_back(eraser);
    _meshes.emplace_back(notebook);

    // main light source mesh (slightly yellow)
    Shapes::setPrismSides(4);
    auto fillLight = Mesh{Shapes::prismVertices, Shapes::prismElements, Texture{_lampShader}};
    fillLight.GlobalPosition = {
            .Translation{326.50f, 1.00f, 35.50f},
    };
    _dLight = { // sets the color and direction of the direction light
            .Direction = {fillLight.GlobalPosition.Translation},
            .Ambient = {0.05f, 0.05f, 0.01f},
            .Diffuse = {1.0f, 1.0f, 0.8f},
    };
    _meshes.emplace_back(fillLight);

    // point light mesh (more yellow)
    auto pointLight = Mesh{Shapes::prismVertices, Shapes::prismElements, Texture{_lampShader}};
    pointLight.GlobalPosition = {
            .Scale{0.1f, 0.1f, 0.1f},
            .Translation{24.0f, 15.0f, 17.0f},
    };
    _pLight = {
            .Position = {pointLight.GlobalPosition.Translation},
            .Diffuse = {0.7f, 0.7f, 0.1f},
            .Specular = {1.0f, 1.0f, 0.0f},
    };
    _meshes.emplace_back(pointLight);


    // sets up the camera's position
//    _camera = Camera(glm::vec3(0.0f, 4.0f, 1.0f));
    _camera = Camera(glm::vec3(10.0f, 2.0f, 10.0f));
//    _camera = Camera(glm::vec3(10.0f, 45.5f, 10.0f), glm::vec3(0.0f, -24.0f, -1.0f));
}

void Application::draw() {
    // rendering the background
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClearColor(0.8f, 0.5f, 0.0f, 1.0f);
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    // DEBUG: ROTATE SPECIFIC MESH
//    int index = 4;
//    auto angle = static_cast<float>(glfwGetTime()) / 50.0f;
//    auto rot = _meshes.at(index).GlobalPosition.Rotation;
//    rot = glm::rotate_slow(rot, glm::radians(angle), {0,1,0});
//    _meshes.at(index).GlobalPosition = {
//            .Scale {_meshes.at(index).GlobalPosition.Scale},
//            .Translation {_meshes.at(index).GlobalPosition.Translation},
//            .Rotation = rot,
//            .RotationAxis {_meshes.at(index).GlobalPosition.RotationAxis},
//            .RotationAngle = _meshes.at(index).GlobalPosition.RotationAngle
//    };

    // apply matrix transformations and shaders to all the object meshes
    for (auto& mesh : _meshes) {
        // model transforms the objects and sets their positions relative to the origin
//        glm::mat4 model = mesh.ModelMatrix;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, mesh.GlobalPosition.Translation);
        model = glm::scale(model, mesh.GlobalPosition.Scale);
        model = glm::rotate(model, mesh.GlobalPosition.RotationAngle, mesh.GlobalPosition.RotationAxis);
        model *= mesh.GlobalPosition.Rotation;

        // sets the required uniform variables for the 3d object view
        mesh.GetShader().Bind();
        mesh.GetShader().SetMat4("model", model);
        mesh.GetShader().SetMat4("view", _camera.View());
        mesh.GetShader().SetMat4("projection", Camera::Projection());

        // sets the uniform material shader properties
        mesh.Material.SendToShader(mesh.GetShader());

        // sets the direct light shader properties
        _dLight.SendToShader(mesh.GetShader());

        // sets the point light shader properties
        _pLight.SendToShader(mesh.GetShader());

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

    // DEBUG: controls first light source position
//    float step = 0.5f;
//    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x, pos.y + step, pos.z};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//    }
//    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x, pos.y - step, pos.z};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//    }
//    if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x - step, pos.y, pos.z};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//    }
//    if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x + step, pos.y, pos.z};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//    }
//    if (glfwGetKey(_window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x, pos.y, pos.z - step};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//    }
//    if (glfwGetKey(_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
//        auto pos = _lightMeshes.at(0).GlobalPosition.Translation;
//        auto newPos = glm::vec3{pos.x, pos.y, pos.z + step};
//        _dLight.Direction = newPos;
//        _lightMeshes.at(0).GlobalPosition.Translation = newPos;
//        std::printf("{%.2ff, %.2ff, %.2ff}\n", pos.x, pos.y, pos.z);
//    }

    // sets the last frame to the time this function runs in the render loop to standardize movement
    Camera::CaptureFrame();
}