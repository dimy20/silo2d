#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ios>
#include <ostream>
#include <vector>
#include <queue>
#include <iostream>
#include <random>
#include <tuple>

#include "err.h"
#include "Renderer.h"
#include "Shader.h"
#include <Physics2D/Particle.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define WINDOW_NAME "Silo"

struct App{
    bool init();
    void draw();
    void update();
    void doInput(GLFWwindow * window);

    std::vector<std::tuple<Physics2D::Particle, float>> mParticles;

};

float rand_f32(){ return static_cast<float>(rand()) / (float)RAND_MAX; }

bool App::init(){
    if(!Renderer::init(WINDOW_W, WINDOW_H, WINDOW_NAME)) return false;

    srand(time(NULL));
    
    for(int i = 0; i < 10; i++){

        int r = rand() % 15;
        int x = rand() % static_cast<int>((Renderer::WinWidth() - r));
        int y = rand() % static_cast<int>((Renderer::WinHeight() - r));
        float mass = rand_f32();

        Physics2D::Particle p(glm::vec2(x, y), mass);
        mParticles.push_back(std::make_tuple(p, r));
    }
    return true;
};

void App::doInput(GLFWwindow * window){
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int f = 0;

std::ostream& operator << (std::ostream& out, const glm::vec2& vec){
    out << "(";
    out << vec.x << ", ";
    out << vec.y << ")";
    return out;
}

void App::draw(){
    for(const auto& p : mParticles){
        auto& particle = std::get<0>(p);

        Renderer::drawCircle(particle.mPosition, std::get<1>(p));
    }
};

void App::update(){
    glm::vec2 windForce(2.0f);

    for(auto& entry : mParticles){
        auto& p = std::get<0>(entry);
        auto radius = std::get<1>(entry);

        glm::vec2 weightForce(0.0f, 9.81f * p.mMass);

        p.applyForce(windForce);
        p.applyForce(weightForce);

        p.integrate(Renderer::deltaTime());

        if(p.mPosition.x - radius <= 0){
            p.mVelocity.x *= -0.8;
            p.mPosition.x = radius;
        }

        if(p.mPosition.x + radius >= Renderer::WinWidth()){
            p.mVelocity.x *= -0.8;
            p.mPosition.x = Renderer::WinWidth() - radius;
        }

        if(p.mPosition.y - radius <= 0){
            p.mVelocity.y *= -0.8f;
            p.mPosition.y = radius;
        }

        if(p.mPosition.y + radius >= Renderer::WinHeight()){
            p.mVelocity.y *= -0.8f;
            p.mPosition.y = Renderer::WinHeight() - radius;
        }

    }
};

int main(){
    App app;

    if(!app.init()){
        exit(1);
    }

    while(!Renderer::should_close()){
        app.doInput(Renderer::getWindow());

        app.update();

        Renderer::clear();

        //Render
        app.draw();

        //Present
        glfwSwapBuffers(Renderer::getWindow());


        Renderer::update();
    };

    Renderer::quit();
};
