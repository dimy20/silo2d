#pragma once

#include <glm/vec3.hpp>

struct GLFWwindow;

namespace Renderer{
    bool init(int w, int h, const char *window_name);
    void clear();
    void quit();
    void update();
    bool should_close();

    void drawTriangle();
    void drawRectangle();

    void drawCircle(const glm::vec3& center, float radius);

    GLFWwindow * get_window();
};
