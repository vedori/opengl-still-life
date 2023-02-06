#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

// The class keeps track of the framing, this standardizes the speed of movement
float Camera::_lastFrame {};
float Camera::_speed {5.0f};
float Camera::_sensitivity {0.05f};

// The offset for the front of the camera
glm::vec3 Camera::_cameraFront {};

// Values for calculating the viewing angle (pitch and yaw)
float Camera::_lastX {};
float Camera::_lastY {};
float Camera::_yaw {-90.0f};
float Camera::_pitch {0.0f};

// Determines whether the perspective is orthogonal
bool Camera::_orthographicMode {false};

// initializes the camera's position and initial values for some attributes
Camera::Camera(glm::vec3 position, glm::vec3 cameraView, float sensitivity) : _cameraPos{position} {
    // sets the initial view to the cameraView
    _cameraFront = cameraView;

    // The first value for _lastX and _lastY is the center of the screen
    int x = 0;
    int y = 0;
    glfwGetWindowSize(glfwGetCurrentContext(), &x, &y);
    _lastX = static_cast<float>(x) / 2.0f;
    _lastY = static_cast<float>(y) / 2.0f;

    // Sets up all callback functions ...

    // mouse scroll adjusts the camera speed
    glfwSetScrollCallback(glfwGetCurrentContext(), [](GLFWwindow *window, double xOffset, double yOffset) {
        const float SPEED_INCREMENT = 0.5f;
        const float HIGHEST_SPEED = 15.0f;
        if (yOffset < 0.0f) {
            if (_speed - SPEED_INCREMENT * 2 > 0.0f) {
                _speed -= SPEED_INCREMENT;
            }
        }
        if (yOffset > 0.0f) {
            if (_speed < HIGHEST_SPEED) {
                _speed += SPEED_INCREMENT;
            }
        }
    });

    // mouse cursor position changes the orientation of the camera to look up/down or right/left
    _sensitivity = sensitivity;
    glfwSetCursorPosCallback(glfwGetCurrentContext(), [](GLFWwindow* window, double xPos, double yPos) {
        float xOffset = static_cast<float>(xPos) - Camera::_lastX;
        float yOffset = static_cast<float>(yPos) - Camera::_lastY;
        Camera::_lastX = static_cast<float>(xPos);
        Camera::_lastY = static_cast<float>(yPos);

        // applies the sensitivity to the offset
        xOffset *= Camera::_sensitivity;
        yOffset *= Camera::_sensitivity;

        // yaw and pitch
        Camera::_yaw += xOffset;
        Camera::_pitch -= yOffset;

        // ensures that the pitch angle's range is [-89,89] inclusive to prevent the screen from being flipped
        if (Camera::_pitch > 89.0f) {
            Camera::_pitch = 89.0f;
        }
        if (Camera::_pitch < -89.0f) {
            Camera::_pitch = -89.0f;
        }

        // sets the front matrix for the view
        {
            using namespace glm;
            vec3 front;
            front.x = cos(radians(Camera::_yaw)) * cos(radians(Camera::_pitch));
            front.y = sin(radians(Camera::_pitch));
            front.z = sin(radians(Camera::_yaw)) * cos(radians(Camera::_pitch));
            Camera::_cameraFront = normalize(front);
        }
    });

    // switches the perspective to orthogonal mode when the key 'P' is released and vice versa
    glfwSetKeyCallback(glfwGetCurrentContext(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
            Camera::_orthographicMode = ! Camera::_orthographicMode;
        }
    });
}

// returns the matrix of the calculated camera view
glm::mat4 Camera::View() {
    return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

// standard timing for input
// this standardizes movement
void Camera::CaptureFrame() {
    _lastFrame = static_cast<float>(glfwGetTime());
}

// calculates the camera position based on the direction given to this method
void Camera::Move(Camera::Movement movement) {
    // _lastFrame always gets the time before this function is ran

    // the current time
    auto currentFrame = static_cast<float>(glfwGetTime());

    // the difference in times which standardizes speed across CPUs
    auto deltaTime = currentFrame - _lastFrame;

    // camera speed with time considered
    float velocity = _speed * deltaTime;

    // calculates the camera position based on the input
    if (movement == Movement::FORWARD) {
        _cameraPos += _cameraFront * velocity;
    }
    if (movement == Movement::BACKWARD) {
        _cameraPos -=  _cameraFront * velocity;
    }
    if (movement == Movement::LEFT) {
        _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * velocity;
    }
    if (movement == Movement::RIGHT) {
        _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * velocity;
    }
    if (movement == Movement::UP) {
        _cameraPos += _cameraUp * velocity;
    }
    if (movement == Movement::DOWN) {
        _cameraPos -= _cameraUp * velocity;
    }
}

glm::mat4 Camera::Projection() {
    // The width and height of the screen
    int width = 0;
    int height = 0;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    auto aspectRatio = static_cast<float>(width) / static_cast<float >(height);

    // the z axis clipping range
    float nearRange = 0.1f;
    float farRange = 150.0f;

    // scaling the orthographic viewing box to fit the overall scale of the scene
    float orthScale = 9.0f;

    if (_orthographicMode) {
        return glm::ortho(-aspectRatio * orthScale, aspectRatio * orthScale, -orthScale,orthScale,
                          -farRange, farRange);
    } else {
        // the perspective sets the fov, the aspect ratio and clipping range of the viewing box
        return glm::perspective(glm::radians(45.0f),aspectRatio,nearRange, farRange);
    }
}
