#include "window.hh"

CASTRWindow::CASTRWindow(int screenHeight, int screenWidth, int windowHeight, int windowWidth, const char* name, GLFWwindow* sharedWindow)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(screenWidth, screenHeight, name, NULL, sharedWindow);
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glfwSwapInterval(1);
    glViewport(0, 0, windowWidth, windowHeight);
}

CASTRWindow::~CASTRWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}