#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <iostream>

#include "err.h"
#include "Renderer.h"
#include "Shader.h"

#define WINDOW_W 800
#define WINDOW_H 600
#define WINDOW_NAME "Silo"

void do_input(GLFWwindow * window){
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

}

int main(){
    if(!Renderer::init(WINDOW_W, WINDOW_H, WINDOW_NAME)) exit(1);
    Shader shader;

    if(!shader.load("shaders/vertex/base.glsl", "shaders/frag/base.glsl")){
        exit(1);
    }



    while(!Renderer::should_close()){
        do_input(Renderer::get_window());

        Renderer::clear();

        //Render here
        shader.use();
        Renderer::drawRectangle();

        glfwSwapBuffers(Renderer::get_window());
    };


    Renderer::quit();
};
