#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#define DEFAULT_CAMERA_SPEED 10.0f

struct GLFWwindow;
struct Camera{

    Camera(glm::vec3 position,
           glm::vec3 up,
           glm::vec3 front,
           float speed = DEFAULT_CAMERA_SPEED);

    glm::mat4 viewMatrix() const;
    void keypressUpdate(float deltaTime, GLFWwindow * window);
    void mouseMoveUpdate(GLFWwindow * window);

    glm::vec3 mPosition;
    glm::vec3 mUp;
    glm::vec3 mFront;

    float mSpeed;
    float mYaw;
    float mPitch;
    float mSens;
    bool mFirstTime;

    float mPrevMouseX;
    float mPrevMouseY;
};
