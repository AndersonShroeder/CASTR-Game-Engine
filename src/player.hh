#pragma once
#include "scene.hh"
#include "renderer.hh"

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



struct CameraPlane
{
    vf2d p1;
    vf2d p2;

    inline CameraPlane  operator += (const vf2d& rhs) {this->p1 += rhs; this->p2 += rhs;}
    inline CameraPlane  operator -= (const vf2d& rhs) {this->p1 -= rhs; this->p2 -= rhs;}

    void rotate(vf2d vOrigin, float angle)
    {
        p1.rotate(vOrigin, angle);
        p2.rotate(vOrigin, angle);
    }
};

class Player
{
private:
    vf2d vPlayer;
    vf2d vRayDir;
    CameraPlane camera;

    float px;
    float py;
    float epx;
    float epy;

    // angle of direction vector
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

    float DDA(Line& line, Line& line3D, int x);

    void drawPlayer(Renderer& renderer);

    void castRays(int FOV);
    
    Line castRaysVertical(int newAngle, double slope);

    Line castRaysHorizontal(int newAngle, double slope);

    void checkKeys();

    void translate(Directions direction);

    void drawDot();

    void drawLine();

    void renderLines(Line line, float size);
};
