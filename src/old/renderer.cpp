// #include "renderer.hh"
// #include "shaders.hh"

// Renderer::Renderer()
// {
//     window = initWindow();
//     shaderProgram = initShader();

//     glGenVertexArrays(1, &VAO);
//     glBindVertexArray(VAO);
// }

// Renderer::~Renderer()
// {
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteProgram(shaderProgram);

//     glfwDestroyWindow(window);
//     glfwTerminate();
// }

// GLFWwindow *Renderer::initWindow()
// {
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     GLFWwindow *window = glfwCreateWindow(SCREEN_WDITH, FULL_SCREEN_HEIGHT, "RayCasterEngine", NULL, NULL);

//     if (window == NULL)
//     {
//         // std::cout << "Failed to create GLFW window" << std::endl;
//         return NULL;
//     }

//     glfwMakeContextCurrent(window);

//     gladLoadGL();

//     glfwSwapInterval(1);
//     glViewport(0, 0, SCREEN_WDITH, SCREEN_HEIGHT);

//     return window;
// }

// GLuint Renderer::initShader()
// {
//     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     glCompileShader(vertexShader);

//     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     glCompileShader(fragmentShader);

//     GLuint shaderProgram = glCreateProgram();

//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }

// void Renderer::createGridVertices(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, GLfloat color[3])
// {
//     GLfloat gridWindowStart = FIRST_WINDOW_LEFT_EDGE;
//     GLfloat gridWindowEnd = FIRST_WINDOW_RIGHT_EDGE;

//     int index = 0;

//     // Add the vertical lines to the vertex buffer
//     for (GLfloat x = gridWindowStart; x <= gridWindowEnd; x += MAP_STEP_SIZE_WIDTH)
//     {
//         // Top vertex
//         gridVertices[index] = x; gridVertices[index + 1] = -1.0f; gridVertices[index + 2] = 0.0f; 
//         gridVertices[index + 3] = color[0]; gridVertices[index + 4] = color[1]; gridVertices[index + 5] = color[2];

//         index += 6;

//         // Bottom vertex
//         gridVertices[index] = x; gridVertices[index + 1] = 1.0f; gridVertices[index + 2] = 0.0f; 
//         gridVertices[index + 3] = color[0]; gridVertices[index + 4] = color[1]; gridVertices[index + 5] = color[2];

//         indices[index/6 - 1] = index/6 - 1;
//         indices[index/6] = index/6;
//     }

//     index = 0;

//     // Add the horizontal lines to the vertex buffer
//     for (GLfloat y = -1.0f; y <= 1.0f; y += MAP_STEP_SIZE_HEIGHT)
//     {
//         // Left vertex
//         gridVertices[index] = gridWindowStart; gridVertices[index + 1] = y; gridVertices[index + 2] = 0.0f; 
//         gridVertices[index + 3] = color[0]; gridVertices[index + 4] = color[1]; gridVertices[index + 5] = color[2];

//         index += 6;

//         // Right vertex
//         gridVertices[index] = gridWindowEnd; gridVertices[index + 1] = y; gridVertices[index + 2] = 0.0f; 
//         gridVertices[index + 3] = color[0]; gridVertices[index + 4] = color[1]; gridVertices[index + 5] = color[2];

//         indices[index/6 - 1] = index/6 - 1;
//         indices[index/6] = index/6;
//     }
// }

// void Renderer::renderPoint(vf2d point, float color[3])
//     {
//         GLfloat vertices[6] = {point.x, point.y, 0.0f, color[0], color[1], color[2]};

//         glBindVertexArray(VAO);

//         GLuint VBO;
//         glGenBuffers(1, &VBO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//         // Set up the vertex attribute pointers
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

//         glPointSize(10.0f);
//         glDrawArrays(GL_POINTS, 0, 1);
//         glPointSize(1.0f);

//         glDeleteBuffers(1, &VBO);

//         glBindVertexArray(0);
//     }

// void Renderer::drawGrid()
// {
//     // Specify the color of the lines
//     GLfloat color[] = {0.0f, 0.0f, 0.0f, 0.0f};

//     glUseProgram(shaderProgram);

//     glBindVertexArray(VAO);

//     glUseProgram(shaderProgram);

//     glBindVertexArray(VAO);

//     // Set the line color
//     GLint colorLoc = glGetUniformLocation(shaderProgram, "lineColor");
//     glUniform4fv(colorLoc, 1, color);

//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;

//     GLfloat windowStart = -1.0f;
//     GLfloat windowEnd = 1.0f;

//     #ifdef GRID_DEMO
//     windowStart = FIRST_WINDOW_LEFT_EDGE;
//     windowEnd = FIRST_WINDOW_RIGHT_EDGE;
//     #endif

//     // Add the vertical lines to the vertex buffer
//     for (GLfloat x = windowStart; x <= windowEnd; x += MAP_STEP_SIZE_WIDTH)
//     {
//         vertices.push_back(x);
//         vertices.push_back(-1.0f);
//         vertices.push_back(0.0f);

//         vertices.push_back(x);
//         vertices.push_back(1.0f);
//         vertices.push_back(0.0f);

//         indices.push_back(vertices.size() / 3 - 2);
//         indices.push_back(vertices.size() / 3 - 1);
//     }

//     // Add the horizontal lines to the vertex buffer
//     for (GLfloat y = -1.0f; y <= 1.0f; y += MAP_STEP_SIZE_HEIGHT)
//     {
//         vertices.push_back(windowStart);
//         vertices.push_back(y);
//         vertices.push_back(0.0f);

//         vertices.push_back(windowEnd);
//         vertices.push_back(y);
//         vertices.push_back(0.0f);

//         indices.push_back(vertices.size() / 3 - 2);
//         indices.push_back(vertices.size() / 3 - 1);
//     }

//     GLuint gridVBO, gridIBO;
//     glGenBuffers(1, &gridVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

//     glGenBuffers(1, &gridIBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

//     glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

//     glDeleteBuffers(1, &gridVBO);
//     glDeleteBuffers(1, &gridIBO);

//     glBindVertexArray(0);
// }

// void Renderer::drawMap(int map[MAP_HEIGHT * MAP_WIDTH])
// {
//     int startx = -1;
//     int starty = 1;

//     glUseProgram(shaderProgram);
//     glBindVertexArray(VAO);

//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;

//     for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++)
//     {
//         GLfloat color[3];
//         if (map[i] == 0)
//         {
//             color[0] = 1.0f;
//             color[1] = 1.0f;
//             color[2] = 1.0f;
//         }

//         else
//         {
//             color[0] = 0.0f;
//             color[1] = 0.0f;
//             color[2] = 0.0f;
//         }

//         int column = i % 10;
//         int row = i / 10;

//         GLfloat top_y = 1 - MAP_STEP_SIZE_HEIGHT * row;
//         GLfloat bottom_y = 1 - MAP_STEP_SIZE_HEIGHT * (row + 1);
//         GLfloat left_x = -1 + MAP_STEP_SIZE_WIDTH * column;
//         GLfloat right_x = -1 + MAP_STEP_SIZE_WIDTH * (column + 1);

//         // TOP LEFT VERTEX
//         vertices.push_back(left_x);
//         vertices.push_back(top_y);
//         vertices.push_back(0.0f);
//         vertices.push_back(color[0]);
//         vertices.push_back(color[1]);
//         vertices.push_back(color[2]);

//         // TOP RIGHT VERTEX
//         vertices.push_back(right_x);
//         vertices.push_back(top_y);
//         vertices.push_back(0.0f);
//         vertices.push_back(color[0]);
//         vertices.push_back(color[1]);
//         vertices.push_back(color[2]);

//         // BOTTOM LEFT VERTEX
//         vertices.push_back(left_x);
//         vertices.push_back(bottom_y);
//         vertices.push_back(0.0f);
//         vertices.push_back(color[0]);
//         vertices.push_back(color[1]);
//         vertices.push_back(color[2]);

//         // BOTTOM RIGHT VERTEX
//         vertices.push_back(right_x);
//         vertices.push_back(bottom_y);
//         vertices.push_back(0.0f);
//         vertices.push_back(color[0]);
//         vertices.push_back(color[1]);
//         vertices.push_back(color[2]);

//         int n = vertices.size() / 6;

//         // TRIANGLE 1
//         indices.push_back(n - 4);
//         indices.push_back(n - 3);
//         indices.push_back(n - 2);

//         // TRIANGLE 2
//         indices.push_back(n - 3);
//         indices.push_back(n - 2);
//         indices.push_back(n - 1);
//     }

//     GLuint gridVBO, gridIBO;
//     glGenBuffers(1, &gridVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

//     glGenBuffers(1, &gridIBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//     // Set up vertex attribute pointers
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
//     glEnableVertexAttribArray(1);

//     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

//     glDeleteBuffers(1, &gridVBO);
//     glDeleteBuffers(1, &gridIBO);

//     glBindVertexArray(0);
// }