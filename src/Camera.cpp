
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>

Camera::Camera(glm::vec3 position,
               glm::vec3 up,
               glm::vec3 front,
               float speed) { 
    mPosition = position;
    mUp = up;
    mFront = front;
    mSpeed = speed;
    mYaw = -90.0f;
    mPitch = 0.0f;
    mSens = 0.1f;
    mFirstTime = true;
    mPrevMouseX = 0.0f;
    mPrevMouseY = 0.0f;
};

void Camera::keypressUpdate(float deltaTime, GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        mPosition += glm::normalize(glm::cross(mUp, mFront)) * mSpeed * deltaTime;
    };

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        mPosition -= glm::normalize(glm::cross(mUp, mFront)) * mSpeed * deltaTime;
    };

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        mPosition += mFront * mSpeed * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        mPosition -= mFront * mSpeed * deltaTime;
    }
}

void Camera::mouseMoveUpdate(GLFWwindow * window){
    float xOffset, yOffset;

    //get mouse x and y position and calculate offsets
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if(mFirstTime){ // avoid sudden movement when mouse enters screen for the first time
        xOffset = 0.0f;
        yOffset = 0.0f;
        mFirstTime = false;
    }else{
        xOffset = mouseX - mPrevMouseX;
        yOffset = -(mouseY - mPrevMouseY);
    }

    mPrevMouseX = mouseX;
    mPrevMouseY = mouseY;

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        //scale offsets by sens and update angles
        float yawOffset = xOffset * mSens;
        float pitchOffset = yOffset * mSens;

        mYaw += yawOffset;
        mPitch += pitchOffset;

        if(mPitch > 89.0f) mPitch = 89.0f;
        if(mPitch < -89.0f) mPitch = -89.0f;

        glm::vec3 newDirection;

        newDirection.x = std::cos(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));
        newDirection.y = std::sin(glm::radians(mPitch));
        newDirection.z = std::sin(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));

        mFront = glm::normalize(newDirection);
    }
}

glm::mat4 Camera::viewMatrix() const{ 
    glm::vec3 target = mPosition + mFront;
    return glm::lookAt(mPosition, target, mUp); 
};
