#include "window.hh"

/**
 * @brief Constructs a new CASTRWindow object.
 *
 * This constructor initializes a GLFW window for rendering using OpenGL.
 *
 * @param screenHeight The height of the screen or rendering area.
 * @param screenWidth The width of the screen or rendering area.
 * @param windowHeight The height of the window to create.
 * @param windowWidth The width of the window to create.
 * @param name The name/title of the window.
 * @param sharedWindow A pointer to an existing GLFW window to share resources with (can be nullptr).
 */
CASTRWindow::CASTRWindow(int screenHeight, int screenWidth, int windowHeight, int windowWidth, const char* name, GLFWwindow* sharedWindow)
{
    // Initialize GLFW
    glfwInit();

    // Set GLFW window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window with the specified parameters
    this->window = glfwCreateWindow(screenWidth, screenHeight, name, NULL, sharedWindow);

    // Store the window dimensions
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;

    // Make the created window the current OpenGL context
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    gladLoadGL();

    // Set the swap interval (vertical sync) to 1 (enabled)
    glfwSwapInterval(1);

    // Set the OpenGL viewport to match the window dimensions
    glViewport(0, 0, windowWidth, windowHeight);
}

/**
 * @brief Destructs the CASTRWindow object.
 *
 * This destructor cleans up resources associated with the window and terminates GLFW.
 */
CASTRWindow::~CASTRWindow()
{
    // Destroy the GLFW window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();
}