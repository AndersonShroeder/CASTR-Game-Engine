# pragma Once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CASTRWindow
{
public:
    GLFWwindow* window;
    int windowHeight, windowWidth;

    CASTRWindow(int screenHeight, int screenWidth, int windowHeight, int windowWidth, const char* name, GLFWwindow* sharedWindow = NULL);
    ~CASTRWindow();
};