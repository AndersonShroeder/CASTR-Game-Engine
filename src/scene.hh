#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define SCREEN_WDITH 800
#define SCREEN_HEIGHT 800
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define XPIXEL 1/SCREEN_WDITH
#define YPIXEL 1/SCREEN_HEIGHT

class Scene
{
    public:
    GLFWwindow *window;
    GLuint shaderProgram;
    GLuint VAO;

    Scene();

    GLFWwindow* initWindow();

    GLuint initShader();
};