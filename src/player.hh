#pragma once
#include "scene.hh"
#include <math.h>
#define PI 3.14159265358
#define ONE_DEGREE_RADIAN PI / 180
#define ONE_DEGREE 1

enum Directions
{
    FORWARD,
    BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,
};

enum ConversionType
{
    VERTICAL,
    HORIZONTAL,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

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

// Converts a normalized x, y coordinate to a grid index.
struct Point
{
    int index;
    int index1;
    int index2;

    Point(float xval, float yval, ConversionType type, ConversionType direction)
    {
        int pixy = (yval + 1) * (SCREEN_HEIGHT)/2.0 + .5f;
        int pixx = (xval + 1) * (SCREEN_WDITH) + 1;

        int xIndex = floor((pixx) / float(CELL_WIDTH*2));
        int yIndex = floor(((pixy) / float(CELL_HEIGHT)));

        switch (type)
        {
        case (HORIZONTAL):
            index = (100 - yIndex * (MAP_HEIGHT)) + xIndex;
            index -= direction == UP ? MAP_WIDTH : 0;
            break;

        case (VERTICAL):
            index = 100 - (yIndex * 10) - (10 - xIndex); 
            index -= direction == RIGHT ? 0 : 1;
            break;

        default:
            break;
        }
    }

    bool intersection(int *map)
    {
        if (index >= 0 && index <= 99)
        {
            if (map[index] != 0)
            {
                return true;
            }
        }

        return false;
    }
};

struct Line
{
    // Verticies is a vector of length 12 containing two points
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indicies;

    float length()
    {
        return sqrt(pow((vertices.at(0) - vertices.at(6)), 2) + pow((vertices.at(1) - vertices.at(7)), 2));
    }
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

    GLuint VAO;
    GLuint shaderProgram;
    int* map;

    GLfloat color[3] = {1.0f, 1.0f, 0.0f};

public:
    Player(float px, float py, GLFWwindow *window, const GLuint &VAO, const GLuint &shaderProgram, int map[MAP_HEIGHT*MAP_WIDTH]);

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

    void drawPlayer();

    void castRays(int FOV);
    
    Line castRaysVertical(int newAngle, double slope);

    Line castRaysHorizontal(int newAngle, double slope);

    void checkKeys();

    void translate(Directions direction);

    void drawDot();

    void drawLine();

    void renderLines(Line line, float size);
};
