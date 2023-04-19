#include "scene.hh"
#include "shaders.hh"

Scene::Scene()
{
    window = initWindow();
    shaderProgram = initShader();

    glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
}

GLFWwindow* Scene::initWindow()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WDITH, SCREEN_HEIGHT, "RayCasterEngine", NULL, NULL);

	if (window == NULL)
	{
		// std::cout << "Failed to create GLFW window" << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, SCREEN_WDITH, SCREEN_HEIGHT);

	return window;
}

GLuint Scene::initShader()
{
	// Create a vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Load the vertex shader source code into the shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	// Create a fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the fragment shader source code into the shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	// Create a shader program object
	GLuint shaderProgram = glCreateProgram();

	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link the shader program
	glLinkProgram(shaderProgram);

	// Delete the vertex and fragment shader objects (no longer needed)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Return the ID of the compiled and linked shader program
	return shaderProgram;
}