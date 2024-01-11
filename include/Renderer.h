#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct GLFWwindow;

namespace Renderer{
    bool init(int w, int h, const char *window_name);
    void clear();
    void quit();
    void update();
    bool shouldClose();
    bool initialized();

    void drawTriangle();
    void drawRectangle(int x, int y, int w, int h, const glm::vec3& color = glm::vec3(1.0f));
    void drawCircle(const glm::vec2& center, float radius, const glm::vec3& color = glm::vec3(1.0f));

    void drawCube(const glm::vec3& position, const glm::vec3& color);

    GLFWwindow * getWindow();
    float deltaTime();
    uint32_t WinWidth();
    uint32_t WinHeight();
};
