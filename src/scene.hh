#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#define FULL_SCREEN_WIDTH 1600
#define FULL_SCREEN_HEIGHT 800
#define SCREEN_WDITH 800
#define SCREEN_HEIGHT 800
#define FIRST_WINDOW_LEFT_EDGE -1.0f
#define FIRST_WINDOW_RIGHT_EDGE 0.0f
#define SECOND_WINDOW_LEFT_EDGE 0.0f
#define SECOND_WINDOW_RIGHT_EDGE 1.0f


#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define CELL_WIDTH (SCREEN_WDITH)/(2*(MAP_WIDTH))
#define CELL_HEIGHT (SCREEN_HEIGHT)/(MAP_HEIGHT)
#define MAP_STEP_SIZE_WIDTH (1.0 / MAP_WIDTH)
#define MAP_STEP_SIZE_HEIGHT (1.0 / MAP_HEIGHT) * 2

#define XPIXEL 2 / SCREEN_WDITH
#define YPIXEL 2 / SCREEN_HEIGHT

#define MOVEMENT_FACTOR 10
#define ROTATION_FACTOR 2

#define NORMAL_TO_PIXEL_X(normal) int((normal + 1) * (SCREEN_WDITH)/2)
#define NORMAL_TO_PIXEL_Y(normal) int((normal + 1) * (SCREEN_HEIGHT)/2)
#define PIXEL_TO_NORMAL(pixel) double(((pixel)*2))/(SCREEN_HEIGHT - 1)

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

    void pollEvents();

    void drawGrid();

    void drawMap(int map[MAP_HEIGHT * MAP_WIDTH]);
};