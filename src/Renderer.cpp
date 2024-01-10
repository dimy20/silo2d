#include <chrono>
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ratio>
#include <stdio.h>
#include <system_error>
#include <vector>
#include <glm/ext.hpp>
#include <thread>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "err.h"
#include "Shader.h"
#define TARGET_FPS 60.0f
const double TARGET_FRAME_DURATION = 1.0f / TARGET_FPS;

enum ShaderType{
    BASIC,
    NUM_SHADERS
};

struct BasicShapeInfo{
    VertexBuffer* mVbo = NULL;
    VertexArray* mVao = NULL;
};

enum BasicShapeType{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    CUBE,
    NUM_SHAPES
};

struct RenderCtx{
    GLFWwindow * window;
    size_t window_w;
    size_t window_h;
    BasicShapeInfo basicShapes[BasicShapeType::NUM_SHAPES];
    glm::mat4 othoProjection;
    Shader shaders[ShaderType::NUM_SHADERS];

    float prevTime;
    float deltaTime;
    float initialized;
};

static RenderCtx renderer;

GLFWwindow * Renderer::getWindow() {return renderer.window; }
bool Renderer::shouldClose() { return glfwWindowShouldClose(renderer.window); }
float Renderer::deltaTime() { return renderer.deltaTime; }
uint32_t Renderer::WinWidth() { return renderer.window_w; }
uint32_t Renderer::WinHeight() { return renderer.window_h; };
bool Renderer::initialized() { return renderer.initialized; };

static void initBasicShapes();
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static BasicShapeInfo buildCircle(int numPoints, float radius, const glm::vec3& center);
static bool initShaders();
static BasicShapeInfo buildRectangle();
//static BasicShapeInfo buildCube();

bool Renderer::init(int w, int h, const char *window_name){
    renderer.initialized = false;
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize Glfw\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer.window_w = w;
    renderer.window_h = h;
    renderer.window = glfwCreateWindow(renderer.window_w, renderer.window_h, window_name, NULL, NULL);

    if(!renderer.window){
        fprintf(stderr, "Failed to create window\n");
        return false;
    }

    glfwMakeContextCurrent(renderer.window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fprintf(stderr, "Failed to initialize glad\n");
        return false;
    }

    glViewport(0, 0, renderer.window_w, renderer.window_h);
    glfwSetFramebufferSizeCallback(renderer.window, framebuffer_size_callback);

    renderer.othoProjection = glm::ortho(0.0f,
                                         static_cast<float>(renderer.window_w), 
                                         static_cast<float>(renderer.window_h), 
                                         0.0f, -1.0f, 1.0f);  

    renderer.prevTime = glfwGetTime();
    renderer.deltaTime = 0.0f;

    initBasicShapes();
    if(!initShaders()) return false;

    return renderer.initialized = true;
};

void Renderer::clear(){
    GL_CALL(glClearColor(3.0f / 255.0, 80.0f / 255.0, 150.0 / 255.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::quit(){
    glfwDestroyWindow(renderer.window);
    glfwTerminate();
    for(int i = 0; i < BasicShapeType::NUM_SHAPES; i++){
        auto& shapeInfo = renderer.basicShapes[i];
        delete shapeInfo.mVbo;
        delete shapeInfo.mVao;
    }
}

BasicShapeInfo buildCube(){
    float vertices[] = {
        // Positions
        -1.0f, -1.0f, -1.0f, // 0
        1.0f, -1.0f, -1.0f,  // 1
        1.0f, 1.0f, -1.0f,   // 2
        -1.0f, 1.0f, -1.0f,  // 3
        -1.0f, -1.0f, 1.0f,  // 4
        1.0f, -1.0f, 1.0f,   // 5
        1.0f, 1.0f, 1.0f,    // 6
        -1.0f, 1.0f, 1.0f    // 7
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,     // Front face
        1, 5, 6, 6, 2, 1,     // Right face
        5, 4, 7, 7, 6, 5,     // Back face
        4, 0, 3, 3, 7, 4,     // Left face
        3, 2, 6, 6, 7, 3,     // Top face
        4, 5, 1, 1, 0, 4      // Bottom face
    };

    BasicShapeInfo cubeInfo;
    (void)vertices;
    (void)indices;

    return cubeInfo;
};

static BasicShapeInfo buildRectangle(){
    BasicShapeInfo rectangleInfo;

    glm::vec3 vertices[] = {
        glm::vec3(0.0f, 1.0f, 0.0),
        glm::vec3(1.0f, 0.0f, 0.0),
        glm::vec3(0.0f, 0.0f, 0.0),

        glm::vec3(0.0f, 1.0f, 0.0),
        glm::vec3(1.0f, 1.0f, 0.0),
        glm::vec3(1.0f, 0.0f, 0.0)
    };

    rectangleInfo.mVbo = new VertexBuffer(vertices, sizeof(vertices), sizeof(vertices) / sizeof(vertices[0]));
    rectangleInfo.mVao = new VertexArray();
    rectangleInfo.mVao->setLayoutAttributeVec3(rectangleInfo.mVbo);

    return rectangleInfo;
};

static void initBasicShapes(){
    BasicShapeInfo triangleInfo;

    glm::vec3 positions[3] = {
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
    };

    triangleInfo.mVao = new VertexArray();
    triangleInfo.mVbo = new VertexBuffer(positions, sizeof(positions), 3);
    triangleInfo.mVao->setLayoutAttributeVec3(triangleInfo.mVbo);

    auto infoCircle = buildCircle(100, 1.0f, glm::vec3(0.0f));
    auto rectangleInfo = buildRectangle();

    renderer.basicShapes[BasicShapeType::TRIANGLE] = triangleInfo;
    renderer.basicShapes[BasicShapeType::RECTANGLE] = rectangleInfo;
    renderer.basicShapes[BasicShapeType::CIRCLE] = infoCircle;
    renderer.basicShapes[BasicShapeType::CUBE] = buildCube();
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void Renderer::drawTriangle(){
    const auto& triangle = renderer.basicShapes[BasicShapeType::TRIANGLE];
    triangle.mVao->bind(); 
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, triangle.mVbo->mCount));
    triangle.mVao->unBind();
};

void Renderer::drawCube(const glm::vec3& position, const glm::vec3& color){

};

void Renderer::drawRectangle(int x, int y, int w, int h, const glm::vec3& color){
    auto& shader = renderer.shaders[ShaderType::BASIC];

    glm::mat4 model(1.0f);


    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(w, h, 1.0f));

    shader.use();
    shader.setMat4("modelMatrix", model);
    shader.setMat4("projectionMatrix", renderer.othoProjection);
    shader.setVec3("color", color);

    auto& rec = renderer.basicShapes[BasicShapeType::RECTANGLE];

    rec.mVao->bind();
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, rec.mVbo->mCount));
    rec.mVao->unBind();
};

void Renderer::drawCircle(const glm::vec2& center, float radius, const glm::vec3& color){
    auto& shader = renderer.shaders[ShaderType::BASIC];
    //Render here
    shader.use();

    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(center, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));

    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", renderer.othoProjection);
    shader.setVec3("color", color);

    const auto& circle = renderer.basicShapes[BasicShapeType::CIRCLE];
    circle.mVao->bind();
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, circle.mVbo->mCount));
    circle.mVao->unBind();
}

static BasicShapeInfo buildCircle(int numPoints, float radius, const glm::vec3& center){
    std::vector<glm::vec3> points;

    //Angle between two adjacent points and the center of the circle
    float angleStep = (2.0f * M_PI) / static_cast<float>(numPoints);
    float alpha = angleStep;

    glm::vec3 v0(center.x + radius, center.y, 0.0f);
    points.push_back(v0);

    for(int i = 0; i < numPoints; i++){
        glm::vec3 nextV(std::cos(alpha) * radius, std::sin(alpha) * radius, 0.0f);

        points.push_back(nextV);
        alpha += angleStep;
    }

    //Get the vertex data
    std::vector<glm::vec3> circleVertexData;
    size_t n = points.size();

    for(int i = 0; i < n - 1; i++){
        circleVertexData.push_back(center);
        circleVertexData.push_back(points[i]);
        circleVertexData.push_back(points[i + 1]);
    };

    BasicShapeInfo circleInfo;
    circleInfo.mVao = new VertexArray();
    size_t count = circleVertexData.size();
    circleInfo.mVbo = new VertexBuffer(&circleVertexData[0], sizeof(glm::vec3) * count, count);
    circleInfo.mVao->setLayoutAttributeVec3(circleInfo.mVbo);

    return circleInfo;
};

static bool initShaders(){
    auto& shader = renderer.shaders[ShaderType::BASIC];
    if(!shader.load("shaders/vertex/base.glsl", "shaders/frag/base.glsl")){
        return false;
    }
    return true;
};

void Renderer::update(){
    float now = glfwGetTime();
    float elapsedTime = now - renderer.prevTime;

    if(elapsedTime >= TARGET_FRAME_DURATION){
        renderer.prevTime = now;
        renderer.deltaTime = elapsedTime;
        return;
    }

    //Frame was faster than target frame time, sleep

    long waitTime = (TARGET_FRAME_DURATION - elapsedTime) * 1000;
    waitTime = std::max(waitTime, 1L);

    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));

    now = glfwGetTime();
    renderer.deltaTime = now - renderer.prevTime;
    renderer.prevTime = now;
};
