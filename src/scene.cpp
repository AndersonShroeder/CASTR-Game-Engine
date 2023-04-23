#include "scene.hh"
#include "shaders.hh"

Scene::Scene()
{
    window = initWindow();
    shaderProgram = initShader();

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
}
Scene::~Scene()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);

    glfwTerminate();
}

GLFWwindow *Scene::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT, "RayCasterEngine", NULL, NULL);

    if (window == NULL)
    {
        // std::cout << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glfwSwapInterval(1);
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

void Scene::drawGrid()
{
    // Specify the color of the lines
    GLfloat color[] = {0.0f, 0.0f, 0.0f, 0.0f};

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    // Set the line color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "lineColor");
    glUniform4fv(colorLoc, 1, color);

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // Add the vertical lines to the vertex buffer
    for (GLfloat x = FIRST_WINDOW_LEFT_EDGE; x <= FIRST_WINDOW_RIGHT_EDGE; x += MAP_STEP_SIZE_WIDTH)
    {
        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);

        vertices.push_back(x);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        indices.push_back(vertices.size() / 3 - 2);
        indices.push_back(vertices.size() / 3 - 1);
    }

    // Add the horizontal lines to the vertex buffer
    for (GLfloat y = -1.0f; y <= 1.0f; y += MAP_STEP_SIZE_HEIGHT)
    {
        vertices.push_back(FIRST_WINDOW_LEFT_EDGE);
        vertices.push_back(y);
        vertices.push_back(0.0f);

        vertices.push_back(FIRST_WINDOW_RIGHT_EDGE);
        vertices.push_back(y);
        vertices.push_back(0.0f);

        indices.push_back(vertices.size() / 3 - 2);
        indices.push_back(vertices.size() / 3 - 1);
    }

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}

void Scene::drawMap(int map[MAP_HEIGHT * MAP_WIDTH])
{
    int startx = -1;
    int starty = 1;

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++)
    {
        GLfloat color[3];
        if (map[i] == 0)
        {
            color[0] = 1.0f;
            color[1] = 1.0f;
            color[2] = 1.0f;
        }

        else
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
        }

        int column = i % 10;
        int row = i / 10;

        GLfloat top_y = 1 - MAP_STEP_SIZE_HEIGHT * row;
        GLfloat bottom_y = 1 - MAP_STEP_SIZE_HEIGHT * (row + 1);
        GLfloat left_x = -1 + MAP_STEP_SIZE_WIDTH * column;
        GLfloat right_x = -1 + MAP_STEP_SIZE_WIDTH * (column + 1);

        // TOP LEFT VERTEX
        vertices.push_back(left_x);
        vertices.push_back(top_y);
        vertices.push_back(0.0f);
        vertices.push_back(color[0]);
        vertices.push_back(color[1]);
        vertices.push_back(color[2]);

        // TOP RIGHT VERTEX
        vertices.push_back(right_x);
        vertices.push_back(top_y);
        vertices.push_back(0.0f);
        vertices.push_back(color[0]);
        vertices.push_back(color[1]);
        vertices.push_back(color[2]);

        // BOTTOM LEFT VERTEX
        vertices.push_back(left_x);
        vertices.push_back(bottom_y);
        vertices.push_back(0.0f);
        vertices.push_back(color[0]);
        vertices.push_back(color[1]);
        vertices.push_back(color[2]);

        // BOTTOM RIGHT VERTEX
        vertices.push_back(right_x);
        vertices.push_back(bottom_y);
        vertices.push_back(0.0f);
        vertices.push_back(color[0]);
        vertices.push_back(color[1]);
        vertices.push_back(color[2]);

        int n = vertices.size() / 6;

        // TRIANGLE 1
        indices.push_back(n - 4);
        indices.push_back(n - 3);
        indices.push_back(n - 2);

        // TRIANGLE 2
        indices.push_back(n - 3);
        indices.push_back(n - 2);
        indices.push_back(n - 1);
    }

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}