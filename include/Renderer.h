#pragma once

struct GLFWwindow;

namespace Renderer{
    bool init(int w, int h, const char *window_name);
    void clear();
    void quit();
    void update();
    bool should_close();
    void drawTriangle();
    void drawRectangle();

    GLFWwindow * get_window();
};
