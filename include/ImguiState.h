#pragma once

struct GLFWwindow;

struct ImguiState{
    void init(GLFWwindow * window);
    void render();

    bool mShowDemoWindow;
    bool mShowAnotherWindow;
    //ImVec4 mClearColor =  ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};
