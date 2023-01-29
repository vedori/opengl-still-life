#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    enum class Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
public:
    Camera() = default;
    explicit Camera(glm::vec3 position, float cameraSensitivity);
    // returns the camera view as a matrix
    glm::mat4 View();
    void Move(Movement movement);
private:
    glm::vec3 _cameraPos;
    static float _speed;
    static float _lastFrame;
    static float _lastX;
    static float _lastY;
    static float _yaw;
    static float _pitch;
    static float _sensitivity;
private:
    // the offset of the front of the camera
    static glm::vec3 _cameraFront;
    // The value for which axis is considered up
    constexpr static const glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
};