#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

// The class keeps track of the framing, this standardizes the speed of movement
float Camera::_lastFrame {0.0f};
float Camera::_speed {20.0f};
float Camera::_sensitivity {0.05f};


glm::vec3 Camera::_cameraFront {glm::vec3(0.0f, 0.0f, -1.0f)};

// Values for calculating the viewing angle (pitch and yaw)
float Camera::_lastX {};
float Camera::_lastY {};
float Camera::_yaw {-90.0f};
float Camera::_pitch {0.0f};

// initializes the camera's position and initial values for some attributes
Camera::Camera(glm::vec3 position, float sensitivity) : _cameraPos{position} {
    // The first value for _lastX and _lastY is the center of the screen
    int x = 0;
    int y = 0;
    glfwGetWindowSize(glfwGetCurrentContext(), &x, &y);
    _lastX = static_cast<float>(x) / 2.0f;
    _lastY = static_cast<float>(y) / 2.0f;

    // Sets up all callback functions ...

    // mouse scroll adjusts the camera speed
    glfwSetScrollCallback(glfwGetCurrentContext(), [](GLFWwindow *window, double xOffset, double yOffset) {
        const float SPEED_INCREMENT = 1.0f;
        const float HIGHEST_SPEED = 200.0f;
        if (yOffset < 0.0f) {
            if (_speed - SPEED_INCREMENT > 0.0f) {
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
        glm::vec3 front;
        front.x = static_cast<float>(cos(glm::radians(Camera::_yaw)) * cos(glm::radians(Camera::_pitch)));
        front.y = static_cast<float>(sin(glm::radians(Camera::_pitch)));
        front.z = static_cast<float>(sin(glm::radians(Camera::_yaw)) * cos(glm::radians(Camera::_pitch)));
        Camera::_cameraFront = glm::normalize(front);
    });
}

// returns the matrix of the calculated camera view
glm::mat4 Camera::View() {
    // standard timing for input
    // the input is processed in the next render loop after the view hence the name lastFrame
    _lastFrame = static_cast<float>(glfwGetTime());

    return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
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