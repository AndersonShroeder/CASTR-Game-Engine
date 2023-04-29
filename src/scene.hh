#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "config.hh"

class Scene
{
public:
    GLFWwindow *window;
    GLuint shaderProgram;
    GLuint VAO;

    Scene();

    ~Scene();

    GLFWwindow *initWindow();

    GLuint initShader();

    void drawGrid();

    void drawMap(int map[MAP_HEIGHT * MAP_WIDTH]);
};