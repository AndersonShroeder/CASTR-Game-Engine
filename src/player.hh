#pragma once
#include "scene.hh"
#include <math.h>
#define PI 3.14159265358
#define ONE_DEGREE_RADIAN PI / 180
#define ONE_DEGREE 1

struct Keys
{
    bool e_key = false;
    bool q_key = false;
    bool w_key = false;
    bool a_key = false;
    bool s_key = false;
    bool d_key = false;
    bool shift_key = false;
};

enum Directions
{
    FORWARD,
    BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,
};

struct Line
{
    // Verticies is a vector of length 12 containing two points
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indicies;
    float length = sqrt(pow((vertices.at(0) - vertices.at(6)), 2) + pow((vertices.at(1) - vertices.at(7)), 2));
};

class Player
{
private:
    float px;
    float py;

    // end points for direction vector
    float epx = px + .1;
    float epy = py;

    // angle of direction vector
    int global_angle = 0;
    int angle = 0;

    Keys keys;

    GLfloat color[3] = {1.0f, 1.0f, 0.0f};

public:
    Player(float px, float py, GLFWwindow *window);

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {

        Player *instance = static_cast<Player *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_E)
        {
            if (action == GLFW_PRESS)
                instance->keys.e_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.e_key = false;
        }

        if (key == GLFW_KEY_Q)
        {
            if (action == GLFW_PRESS)
                instance->keys.q_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.q_key = false;
        }

        if (key == GLFW_KEY_W)
        {
            if (action == GLFW_PRESS)
                instance->keys.w_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.w_key = false;
        }

        if (key == GLFW_KEY_S)
        {
            if (action == GLFW_PRESS)
                instance->keys.s_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.s_key = false;
        }

        if (key == GLFW_KEY_A)
        {
            if (action == GLFW_PRESS)
                instance->keys.a_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.a_key = false;
        }

        if (key == GLFW_KEY_D)
        {
            if (action == GLFW_PRESS)
                instance->keys.d_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.d_key = false;
        }

        if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
        {
            if (action == GLFW_PRESS)
                instance->keys.shift_key = true;
            if (action == GLFW_RELEASE)
                instance->keys.shift_key = false;
        }
    }

    void drawPlayer(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH]);

    void drawDot(GLuint &VAO, GLuint &shaderProgram);

    void drawLine(GLuint &VAO, GLuint &shaderProgram);

    void checkKeys();

    void translate(Directions direction);

    void castRays(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH]);

    Line castRaysVertical(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH]);

    Line castRaysHorizontal(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH]);

    void renderLines(GLuint &VAO, GLuint &shaderProgram, std::vector<GLuint> indicies, std::vector<GLfloat> vertices, float size);
};
