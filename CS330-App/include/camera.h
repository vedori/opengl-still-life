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

    // sets up the camera with its position, view, and sensitivity
    explicit Camera(glm::vec3 position, glm::vec3 cameraView = glm::vec3(0.0f, 0.0f, -1.0f),
                    float cameraSensitivity = 0.05f);

    // returns the camera view as a matrix
    glm::mat4 View();

    // returns the projection which sets the fov, the aspect ratio and clipping range of the viewing box
    static glm::mat4 Projection();

    // sets the frame for _lastFrame which standardizes speed across CPUs
    static void CaptureFrame();

    // calculates view when given a movement
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
    static bool _orthographicMode;
private:
    // the offset for the front of the camera view
    static glm::vec3 _cameraFront;

    // The value for which axis is considered up
    constexpr static const glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
};