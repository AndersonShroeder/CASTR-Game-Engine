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


// void CASTRRayCastRenderer::DDA(Line& line, Line& line3D, int x)
// {
//     // ALL VECTORS ARE IN TERMS OF GRID BLOCKS
//     #ifdef NOANGLE
//     double cameraX = x/double(SCREEN_WDITH); // Normalized camera position ON THE CAMERA PLANE
//     const vf2d vRayStart{((camera.p2.x + (camera.p1.x - camera.p2.x) * cameraX) + 1)*10/2, ((camera.p2.y + (camera.p1.y-camera.p2.y) * cameraX)+ 1)*10/2}; // Find start position by
//     vf2d vRayDirReal = ((vf2d{(vRayDir.x - vPlayer.x)*10/2, (vRayDir.y - vPlayer.y)*10/2}).norm());
//     #else
//     vf2d vRayStart{(vPlayer.x + 1)*10/2, (vPlayer.y + 1)*10/2};
//     vf2d vRayDirTemp = vRayDir;
//     vRayDirTemp.rotate(vPlayer, (x*90.0/float(SCREEN_WDITH))*(ONE_DEGREE_RADIAN));  
//     vf2d vRayDirReal= (vf2d{(vRayDirTemp.x + 1)*10/2, (vRayDirTemp.y + 1)*10/2} - vRayStart).norm();
//     #endif

//     const vf2d vRayUnitStepSize = {abs(1/vRayDirReal.x), abs(1/vRayDirReal.y)};

//     vi2d vMapCheck{vRayStart.x, vRayStart.y};
//     vf2d vRayLength1D;
//     vi2d vStep;

//     // If direction is negative we are taking negative steps
//     if (vRayDirReal.x < 0)
//     {
//         vStep.x = -1;
//         vRayLength1D.x = (vRayStart.x - (vMapCheck.x)) * vRayUnitStepSize.x;
//     }

//     else
//     {
//         vStep.x = 1;
//         vRayLength1D.x = (vMapCheck.x + 1.0 - vRayStart.x) * vRayUnitStepSize.x;
//     }
    
//     if (vRayDirReal.y < 0)
//     {
//         vStep.y = -1;
//         vRayLength1D.y = (vRayStart.y - vMapCheck.y) * vRayUnitStepSize.y;
//     }

//     else
//     {
//         vStep.y = 1;
//         vRayLength1D.y = (vMapCheck.y + 1.0 - vRayStart.y) * vRayUnitStepSize.y;
//     }

//     bool tileFound = false;
//     int side = 0;
//     float fMaxDistance = 20.0f;
//     float fDistance = 0.0f;
//     while (!tileFound && fDistance < fMaxDistance)
//     {
//         if (vRayLength1D.x < vRayLength1D.y)
//         {
//             vMapCheck.x += vStep.x;
//             fDistance = vRayLength1D.x;
//             vRayLength1D.x += vRayUnitStepSize.x;
//             side = 0;
//         }
//         else
//         {
//             vMapCheck.y += vStep.y;
//             fDistance = vRayLength1D.y;
//             vRayLength1D.y += vRayUnitStepSize.y;
//             side = 1;
//         }

//         if (vMapCheck.x >= 0 && vMapCheck.x < MAP_WIDTH && vMapCheck.y >= 0 && vMapCheck.y < MAP_HEIGHT)
//         {
//             if (map[(9-vMapCheck.y) * (MAP_WIDTH) + vMapCheck.x] != 0)
//             {
//                 tileFound = true;
//             }
//         }
//     }

//     vf2d vIntersection;
//     if (tileFound)
//     {
//         vIntersection = vRayStart + vRayDirReal * fDistance;
//         vIntersection.x = (vIntersection.x)*(MAP_STEP_SIZE_WIDTH) - 1;
//         vIntersection.y = (vIntersection.y)*(MAP_STEP_SIZE_WIDTH) - 1;
//         vf2d vDistance = vRayLength1D - vRayUnitStepSize;
//         float distance;
//         if (side == 0) distance = vDistance.x;
//         else distance = vDistance.y;
//         float lineHeight = 1 / (4*distance); if (lineHeight > 1){lineHeight = 1;}
//         float color[3] = {1.0f, 0.0f, 0.0f};

//         switch(map[(9-vMapCheck.y) * (MAP_WIDTH) + vMapCheck.x])
//         {
//             case (1):
//                 if (side == 0)
//                 {
//                     color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
//                 }
//                 else
//                 {
//                     color[0] = 0.5f; color[1] = 0.0f; color[2] = 0.0f;
//                 }
//                 break;
//             case (2):
//                 if (side == 0)
//                 {
//                     color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
//                 }
//                 else
//                 {
//                     color[0] = 0.0f; color[1] = 0.5f; color[2] = 0.0f;
//                 }
//                 break;
//             case (3):
//                 if (side == 0)
//                 {
//                     color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
//                 }
//                 else
//                 {
//                     color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.5f;
//                 }
//                 break;
//             default:
//                 break;
//         }
//         // vf2d{vRayStart.x*2/10 - 1, vRayStart.y*2/10 - 1}
//         line.addLine(vIntersection, vPlayer, color);
//         line3D.addLine(vf2d{(-float(XPIXEL) * float(x)), lineHeight}, vf2d{(-float(XPIXEL) * float(x)), -lineHeight}, color);

//         return distance;
//     }

//     return 0;
// }

void CASTRRayCastMapRenderer::createGridVertices(std::vector<GLfloat> color, float stepSize)
{
    std::vector<GLfloat> gridVertices; std::vector<GLuint> gridIndices;

    int index = 0;

    // Add the vertical lines to the vertex buffer
    for (GLfloat x = -1.0f + 2*stepSize; x <= 1.0f - 2*stepSize; x += stepSize)
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
    for (GLfloat y = -1.0f  + 2*stepSize; y < 1.0f - 2*stepSize; y += stepSize)
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

    this->gridLines = Lines(gridVertices, gridIndices, color, 4);
}

void CASTRRayCastMapRenderer::drawGrid()
{
    Point p1({0, 0}, {1.0f, 0.0f, 0.0f}, 20);
    renderGeometry(gridLines);
    renderGeometry(p1);
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

void CASTRRayCastMapRenderer::createMapVertices(int mapWidth, int mapHeight, std::vector<GLfloat> color)
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
}
