#include "geometry.hh"

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

class Entity
{
public:
    vf2d vPos;

    void updatePos(float x, float y)
    {
        vPos.x += x;
        vPos.y += y;
    }

};

class Player : public Entity
{
public:
    vf2d vDir;
    Keys keys;
    
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

    void checkInput();

    void rotate();
};