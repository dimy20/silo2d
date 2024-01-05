#pragma once

#include <glm/vec2.hpp>

struct GLFWwindow;

namespace Renderer{
    bool init(int w, int h, const char *window_name);
    void clear();
    void quit();
    void update();
    bool should_close();

    void drawTriangle();
    void drawRectangle();
    void drawCircle(const glm::vec2& center, float radius);

    GLFWwindow * getWindow();
    float deltaTime();
    uint32_t WinWidth();
    uint32_t WinHeight();
};
