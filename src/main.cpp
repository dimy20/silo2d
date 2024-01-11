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

#include "ImguiState.h"
#include "err.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

#include <Physics2D/Particle.h>
#include <Physics2D/Forces.h>

#define WINDOW_W 1024
#define WINDOW_H 768
#define WINDOW_NAME "Silo"

struct GameParticle{
    Physics2D::Particle particle;
    float radius;
    bool onWater(const glm::vec4& waterDimensions);
};

bool GameParticle::onWater(const glm::vec4& waterDimensions){
    const auto& pos = particle.mPosition;

    int left = waterDimensions.x;
    int right = waterDimensions.x + waterDimensions.z;
    int top = waterDimensions.y;
    int bottom = waterDimensions.y + waterDimensions.w;

    return pos.x > left && pos.x < right && pos.y + radius > top && pos.y - radius < bottom;
}

std::ostream& operator << (std::ostream& out, const glm::vec2& vec){
    out << "(";
    out << vec.x << ", ";
    out << vec.y << ")";
    return out;
}

struct App{
    App();

    bool init();
    void draw();
    void update();
    void doInput(GLFWwindow * window);

    std::vector<GameParticle> mParticles;
    glm::vec2 mMousePos;
    glm::vec4 mWaterDimensions;
    Camera mCamera;
};

float rand_f32(){ return static_cast<float>(rand()) / (float)RAND_MAX; }

App::App() : mCamera(glm::vec3(0.0, 0.0, 2.0f), 
                     glm::vec3(0.0, 1.0, 0.0), 
                     glm::vec3(0.0, 0.0, -1.0f)){
}

bool App::init(){
    if(!Renderer::init(WINDOW_W, WINDOW_H, WINDOW_NAME)) return false;

    srand(time(NULL));
    
    for(int i = 0; i < 10; i++){
        int minSize = 5;
        int maxSize = 20;

        int r = minSize + (maxSize - minSize) * rand_f32();
        int x = rand() % static_cast<int>((Renderer::WinWidth() - r));
        int y = rand() % static_cast<int>((Renderer::WinHeight() - r));
        float mass = rand_f32();

        GameParticle p;
        p.particle = Physics2D::Particle(glm::vec2(x, y), mass);
        p.radius = r;

        mParticles.push_back(p);
    }

    mWaterDimensions = glm::vec4(0.0, 
                                 Renderer::WinHeight() / 2, 
                                 Renderer::WinWidth(), 
                                 Renderer::WinHeight() / 2);



    return true;
};

void App::doInput(GLFWwindow * window){
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if(state == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mMousePos = glm::vec2(xpos, ypos);
    }else if(state == GLFW_RELEASE){
        mMousePos = glm::vec2(0.0f);
    }

}

void App::draw(){
    /*
    glm::vec3 recColor = glm::vec3(77, 93, 117) / 255.0f;

    Renderer::drawRectangle(mWaterDimensions.x,
                            mWaterDimensions.y,
                            mWaterDimensions.z,
                            mWaterDimensions.w, recColor);

    for(const auto& gameParticle: mParticles){
        //auto& gameParticle = std::get<0>(p);
        Renderer::drawCircle(gameParticle.particle.mPosition, gameParticle.radius, glm::vec3(1.0, 0, 0));
    }
    */
    glm::vec3 color(0.6, 0.7, 0.4);
    Renderer::setView(mCamera.viewMatrix());
    Renderer::drawCube(glm::vec3(0.0f, 0.0f, 0.0f), color);
};

void App::update(){
    glm::vec2 windForce(2.0f);

    for(auto& gameParticle: mParticles){
        auto& p = gameParticle.particle;
        auto radius = gameParticle.radius;

        p.applyForce(windForce * Physics2D::PIXELS_PER_METER);
        p.applyForce(Physics2D::weightForce(p) * Physics2D::PIXELS_PER_METER);
        //p.applyForce(Physics2D::frictionForce(p, 10.0f * Physics2D::PIXELS_PER_METER));

        if(mMousePos != glm::vec2(0.0f)){
            p.applyForce(glm::normalize(mMousePos - p.mPosition) * 20.0f * Physics2D::PIXELS_PER_METER);
        }

        if(gameParticle.onWater(mWaterDimensions)){
            p.applyForce(Physics2D::dragForce(p, 0.001) * Physics2D::PIXELS_PER_METER);
        }

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

    mCamera.keypressUpdate(Renderer::deltaTime(), Renderer::getWindow());
    mCamera.mouseMoveUpdate(Renderer::getWindow());
};

int main(){
    App app;

    if(!app.init()){
        exit(1);
    }

    //ImguiState imgui;
    //imgui.init(Renderer::getWindow());

    while(!Renderer::shouldClose()){
        app.doInput(Renderer::getWindow());

        app.update();

        Renderer::clear();

        //Render
        //imgui.render();

        app.draw();

        //Present
        glfwSwapBuffers(Renderer::getWindow());


        Renderer::update();
    };

    Renderer::quit();
};
