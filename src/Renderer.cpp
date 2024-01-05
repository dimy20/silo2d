#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ratio>
#include <stdio.h>
#include <system_error>
#include <vector>
#include <glm/ext.hpp>

#include <thread>

#include "Renderer.h"
#include "err.h"
#include "Shader.h"

#define TARGET_FPS 60.0f
const double TARGET_FRAME_DURATION = 1.0f / TARGET_FPS;

enum ShaderType{
    BASIC,
    NUM_SHADERS
};

struct BasicShapeInfo{
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t numVertices;
};

enum BasicShapeType{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    NUM_SHAPES
};

struct RenderCtx{
    GLFWwindow * window;
    size_t window_w;
    size_t window_h;
    BasicShapeInfo basicShapes[BasicShapeType::NUM_SHAPES];
    glm::mat4 othoProjection;
    Shader shaders[ShaderType::NUM_SHADERS];

    double prevTime;
    double deltaTime;
};

static RenderCtx renderer;

GLFWwindow * Renderer::getWindow() {return renderer.window; }
bool Renderer::should_close() { return glfwWindowShouldClose(renderer.window); }
double Renderer::deltaTime() { return renderer.deltaTime; }

static void initBasicShapes();
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static BasicShapeInfo buildCircle(int numPoints, float radius, const glm::vec3& center);
static bool initShaders();

bool Renderer::init(int w, int h, const char *window_name){

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

    return true;
};

void Renderer::clear(){
    GL_CALL(glClearColor(3.0f / 255.0, 80.0f / 255.0, 150.0 / 255.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::quit(){
    glfwDestroyWindow(renderer.window);
    glfwTerminate();
}

static void initBasicShapes(){
    BasicShapeInfo triangleInfo;

    glm::vec3 positions[3] = {
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
    };

    GL_CALL(glGenBuffers(1, &triangleInfo.vbo));
    GL_CALL(glGenVertexArrays(1, &triangleInfo.vao));

    GL_CALL(glBindVertexArray(triangleInfo.vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, triangleInfo.vbo));

    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0));

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glBindVertexArray(0));

    BasicShapeInfo rectangleInfo;

    glm::vec3 rectangleVertexData[4] = {
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),
    };

    uint32_t rectangleIndices[6] = {
        0, 1, 2,
        0, 2, 3
    };

    GL_CALL(glGenBuffers(1, &rectangleInfo.vbo));
    GL_CALL(glGenBuffers(1, &rectangleInfo.ebo));
    GL_CALL(glGenVertexArrays(1, &rectangleInfo.vao));

    GL_CALL(glBindVertexArray(rectangleInfo.vao));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleInfo.ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rectangleInfo.vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertexData), rectangleVertexData, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glBindVertexArray(0));

    auto infoCircle = buildCircle(100, 1.0f, glm::vec3(0.0f));

    renderer.basicShapes[BasicShapeType::TRIANGLE] = triangleInfo;
    renderer.basicShapes[BasicShapeType::RECTANGLE] = rectangleInfo;
    renderer.basicShapes[BasicShapeType::CIRCLE] = infoCircle;
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void Renderer::drawTriangle(){
    const auto& triangle = renderer.basicShapes[BasicShapeType::TRIANGLE];

    GL_CALL(glBindVertexArray(triangle.vao));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
    GL_CALL(glBindVertexArray(0));

};

void Renderer::drawRectangle(){
    const auto& rec = renderer.basicShapes[BasicShapeType::RECTANGLE];
    GL_CALL(glBindVertexArray(rec.vao));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GL_CALL(glBindVertexArray(0));
};

void Renderer::drawCircle(const glm::vec3& center, float radius){
    auto& shader = renderer.shaders[ShaderType::BASIC];
    //Render here
    shader.use();

    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, center);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));

    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", renderer.othoProjection);

    const auto& circle = renderer.basicShapes[BasicShapeType::CIRCLE];
    GL_CALL(glBindVertexArray(circle.vao));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, circle.numVertices));
    GL_CALL(glBindVertexArray(0));
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

    GL_CALL(glGenBuffers(1, &circleInfo.vbo));
    GL_CALL(glGenVertexArrays(1, &circleInfo.vao));

    GL_CALL(glBindVertexArray(circleInfo.vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, circleInfo.vbo));

    GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                         circleVertexData.size() * sizeof(glm::vec3),
                         &circleVertexData[0],
                         GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glBindVertexArray(0));

    circleInfo.numVertices = circleVertexData.size();

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
    double now = glfwGetTime();
    double elapsedTime = now - renderer.prevTime;

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
