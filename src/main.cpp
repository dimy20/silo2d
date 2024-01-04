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
    int numCircles = 10;

    int yStep = WINDOW_H / numCircles;
    int step = WINDOW_W / numCircles;
    std::cout << yStep << "\n";
    int r = 20;
 
    std::vector<std::vector<glm::vec3>> centers(10, std::vector<glm::vec3>(10, glm::vec3(1.0f)));

    int y = r + 20;

    for(int i = 0; i < 10; i++){
        int x = r + 20;
        for(int j = 0; j < 10; j++){
            centers[i][j] = glm::vec3(x, y, 0.0f);
            x += step + r;
        }
        y += yStep;
    };

    while(!Renderer::should_close()){
        do_input(Renderer::get_window());

        Renderer::clear();

        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                Renderer::drawCircle(centers[i][j], r);
            }
        }

        glfwSwapBuffers(Renderer::get_window());
    };


    Renderer::quit();
};
