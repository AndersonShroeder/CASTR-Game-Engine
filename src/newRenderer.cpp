#include "newRenderer.hh"
#include "shaders.hh"

CASTRRenderer::CASTRRenderer(CASTRWindow *window)
{
    glfwMakeContextCurrent(window->window);
    shaderProgram = initShaderProgram();
    this->window = window;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

CASTRRenderer::~CASTRRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

void CASTRRenderer::render()
{
    glfwMakeContextCurrent(window->window);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.2, 0.2, 1);
}

GLuint CASTRRenderer::initShaderProgram()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

template <class T>
void CASTRRenderer::renderGeometry(T geometry)
{
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(GLfloat), geometry.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indicies.size() * sizeof(GLuint), geometry.indicies.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    geometry.setSize();
    glDrawElements(geometry.type, geometry.indicies.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}

void CASTRRayCastMapRenderer::generateMap(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY, std::vector<GLfloat> gridColor)
{
    createGridVertices(gridColor, stepSizeX, stepSizeY);
    createMapVertices(mapWidth, mapHeight, map, stepSizeX, stepSizeY);
}

void CASTRRayCastMapRenderer::render()
{
    CASTRRenderer::render();
    drawMap();
    drawGrid();
}

void CASTRRayCastMapRenderer::drawGrid()
{
    renderGeometry(gridLines);
}

void CASTRRayCastMapRenderer::drawMap()
{
    renderGeometry(mapTriangles);
}

void CASTRRayCastMapRenderer::createGridVertices(std::vector<GLfloat> color, float stepSizeX, float stepSizeY)
{
    std::vector<GLfloat> gridVertices; std::vector<GLuint> gridIndices;

    int index = 0;

    // Add the vertical lines to the vertex buffer
    for (GLfloat x = -1.0f + stepSizeX; x <= 1.0f - stepSizeX; x += stepSizeX)
    {
        gridVertices.insert(
            gridVertices.end(),
            {
                x, -1.0f, 0.0f, color[0], color[1], color[2], 
                x, 1.0f, 0.0f, color[0], color[1], color[2], 
            }
        );

        gridIndices.push_back(index++);
        gridIndices.push_back(index++);
    }

    // Add the horizontal lines to the vertex buffer
    for (GLfloat y = -1.0f  + stepSizeY; y < 1.0f - stepSizeY; y += stepSizeY)
    {
        gridVertices.insert(
            gridVertices.end(),
            {
                -1.0f, y, 0.0f, color[0], color[1], color[2],
                1.0f, y, 0.0f, color[0], color[1], color[2], 
            }
        );

        gridIndices.push_back(index++);
        gridIndices.push_back(index++);
    }

    this->gridLines = Lines(gridVertices, gridIndices, 4);
}

void CASTRRayCastMapRenderer::createMapVertices(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY)
{
    std::vector<GLfloat> mapVerticies; std::vector<GLuint> mapIndices;
    unsigned int index = 0;

    for (int i = 0; i < mapWidth * mapHeight; i++)
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

        GLfloat yTop = 1 - stepSizeY * row;
        GLfloat yBottom = 1 - stepSizeY * (row + 1);
        GLfloat xLeft = -1 + stepSizeX * column;
        GLfloat xRight = -1 + stepSizeX * (column + 1);


        mapVerticies.insert(
            mapVerticies.end(),
            {
                // TOP
                xLeft, yTop, 0.0f, color[0], color[1], color[2],
                xRight, yTop, 0.0f, color[0], color[1], color[2],

                // BOTTOM
                xLeft, yBottom, 0.0f, color[0], color[1], color[2],
                xRight, yBottom, 0.0f, color[0], color[1], color[2],
            }
        );

        unsigned int n = mapVerticies.size() / 6;

        mapIndices.insert(
            mapIndices.end(),
            {
                n-4, n-3, n-2,
                n-3, n-2, n-1
            }
        );
    }

    this->mapTriangles = Triangles{mapVerticies, mapIndices};
}
