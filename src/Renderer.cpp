
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Renderer.h"
#include "err.h"
#include <glm/glm.hpp>

struct BasicShapeInfo{
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
};

enum BasicShapeType{
    TRIANGLE,
    RECTANGLE,
    NUM_SHAPES
};

struct RenderCtx{
    GLFWwindow * window;
    size_t window_w;
    size_t window_h;
    BasicShapeInfo basicShapes[BasicShapeType::NUM_SHAPES];
};

static RenderCtx renderer;

GLFWwindow * Renderer::get_window() {return renderer.window; }
bool Renderer::should_close() { return glfwWindowShouldClose(renderer.window); }

static void initBasicShapes();
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    initBasicShapes();

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

    renderer.basicShapes[BasicShapeType::TRIANGLE] = triangleInfo;
    renderer.basicShapes[BasicShapeType::RECTANGLE] = rectangleInfo;
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
