#include "player.hh"
#include "math.h"

Player::Player(float px, float py, GLFWwindow *window, const GLuint &VAO, const GLuint &shaderProgram, int map[MAP_HEIGHT*MAP_WIDTH])
{
    this->vPlayer = vf2d{px, py};
    this->vRayDir = vf2d{px + .05, py};
    this->camera = CameraPlane{{px, py -.05f}, {px, py +.05f}};
    this->px = px;
    this->py = py;
    this->VAO = VAO;
    this->shaderProgram = shaderProgram;
    this->map = map;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
}

void Player::drawPlayer(Renderer& renderer)
{
    checkKeys();
    // renderer.renderPoint(vPlayer, color);
    drawLine();
    castRays(1);
}

// #define NOANGLE

float Player::DDA(Line& line, Line& line3D, int x)
{
    // ALL VECTORS ARE IN TERMS OF GRID BLOCKS
    #ifdef NOANGLE
    double cameraX = x/double(SCREEN_WDITH); // Normalized camera position ON THE CAMERA PLANE
    const vf2d vRayStart{((camera.p2.x + (camera.p1.x - camera.p2.x) * cameraX) + 1)*10/2, ((camera.p2.y + (camera.p1.y-camera.p2.y) * cameraX)+ 1)*10/2}; // Find start position by
    vf2d vRayDirReal = ((vf2d{(vRayDir.x - vPlayer.x)*10/2, (vRayDir.y - vPlayer.y)*10/2}).norm());
    #else
    vf2d vRayStart{(vPlayer.x + 1)*10/2, (vPlayer.y + 1)*10/2};
    vf2d vRayDirTemp = vRayDir;
    vRayDirTemp.rotate(vPlayer, (x*90.0/float(SCREEN_WDITH))*(ONE_DEGREE_RADIAN));  
    vf2d vRayDirReal= (vf2d{(vRayDirTemp.x + 1)*10/2, (vRayDirTemp.y + 1)*10/2} - vRayStart).norm();
    #endif

    const vf2d vRayUnitStepSize = {abs(1/vRayDirReal.x), abs(1/vRayDirReal.y)};

    vi2d vMapCheck{vRayStart.x, vRayStart.y};
    vf2d vRayLength1D;
    vi2d vStep;

    // If direction is negative we are taking negative steps
    if (vRayDirReal.x < 0)
    {
        vStep.x = -1;
        vRayLength1D.x = (vRayStart.x - (vMapCheck.x)) * vRayUnitStepSize.x;
    }

    else
    {
        vStep.x = 1;
        vRayLength1D.x = (vMapCheck.x + 1.0 - vRayStart.x) * vRayUnitStepSize.x;
    }
    
    if (vRayDirReal.y < 0)
    {
        vStep.y = -1;
        vRayLength1D.y = (vRayStart.y - vMapCheck.y) * vRayUnitStepSize.y;
    }

    else
    {
        vStep.y = 1;
        vRayLength1D.y = (vMapCheck.y + 1.0 - vRayStart.y) * vRayUnitStepSize.y;
    }

    bool tileFound = false;
    int side = 0;
    float fMaxDistance = 20.0f;
    float fDistance = 0.0f;
    while (!tileFound && fDistance < fMaxDistance)
    {
        if (vRayLength1D.x < vRayLength1D.y)
        {
            vMapCheck.x += vStep.x;
            fDistance = vRayLength1D.x;
            vRayLength1D.x += vRayUnitStepSize.x;
            side = 0;
        }
        else
        {
            vMapCheck.y += vStep.y;
            fDistance = vRayLength1D.y;
            vRayLength1D.y += vRayUnitStepSize.y;
            side = 1;
        }

        if (vMapCheck.x >= 0 && vMapCheck.x < MAP_WIDTH && vMapCheck.y >= 0 && vMapCheck.y < MAP_HEIGHT)
        {
            if (map[(9-vMapCheck.y) * (MAP_WIDTH) + vMapCheck.x] != 0)
            {
                tileFound = true;
            }
        }
    }

    vf2d vIntersection;
    if (tileFound)
    {
        vIntersection = vRayStart + vRayDirReal * fDistance;
        vIntersection.x = (vIntersection.x)*(MAP_STEP_SIZE_WIDTH) - 1;
        vIntersection.y = (vIntersection.y)*(MAP_STEP_SIZE_WIDTH) - 1;
        vf2d vDistance = vRayLength1D - vRayUnitStepSize;
        float distance;
        if (side == 0) distance = vDistance.x;
        else distance = vDistance.y;
        float lineHeight = 1 / (2*distance); if (lineHeight > 1){lineHeight = 1;}
        float color[3] = {1.0f, 0.0f, 0.0f};

        switch(map[(9-vMapCheck.y) * (MAP_WIDTH) + vMapCheck.x])
        {
            case (1):
                if (side == 0)
                {
                    color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
                }
                else
                {
                    color[0] = 0.5f; color[1] = 0.0f; color[2] = 0.0f;
                }
                break;
            case (2):
                if (side == 0)
                {
                    color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
                }
                else
                {
                    color[0] = 0.0f; color[1] = 0.5f; color[2] = 0.0f;
                }
                break;
            case (3):
                if (side == 0)
                {
                    color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
                }
                else
                {
                    color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.5f;
                }
                break;
            default:
                break;
        }
        // vf2d{vRayStart.x*2/10 - 1, vRayStart.y*2/10 - 1}
        // line.addLine(vIntersection, vPlayer, color);
        line3D.addLine(vf2d{(-float(XPIXEL) * float(x)), lineHeight}, vf2d{(-float(XPIXEL) * float(x)), -lineHeight}, color);

        return distance;
    }

    return 0;
}

void Player::castRays(int FOV)
{
    std::vector<GLfloat> verticies;
    std::vector<GLuint> indicies;
    Line line;
    Line line3D;

    unsigned int index = 0;
    for (int x = -SCREEN_WDITH/2; x <= SCREEN_WDITH/2; x++)
    {
        DDA(line, line3D, x);
    }

    
    renderLines(line, 3);
    renderLines(line3D, 1);
}

void Player::checkKeys()
{
    if (keys.e_key)
        angle = angle - ONE_DEGREE * ROTATION_FACTOR;
        if (angle < 0)
        {
            angle += 360;
        }
    if (keys.q_key)
        angle = (angle + ONE_DEGREE * ROTATION_FACTOR) % 360;
    if (keys.w_key)
        translate(FORWARD);
    if (keys.s_key)
        translate(BACKWARD);
    if (keys.a_key)
        translate(STRAFE_LEFT);
    if (keys.d_key)
        translate(STRAFE_RIGHT);
}

void Player::translate(Directions direction)
{
    switch (direction)
    {
    case (FORWARD):
    {
        vf2d vShift{(vRayDir - vPlayer) * XPIXEL * MOVEMENT_FACTOR};
        vPlayer += vShift; 
        vRayDir += vShift; 
        camera += vShift;

        break;
    }

    case (BACKWARD):
    {
        vf2d vShift{(vRayDir - vPlayer) * XPIXEL * MOVEMENT_FACTOR};
        vPlayer -= vShift;
        vRayDir -= vShift; 
        camera -= vShift;

        break;
    }

    case (STRAFE_LEFT):
    {
        vf2d vShift{(vRayDir - vPlayer).perp() * XPIXEL * MOVEMENT_FACTOR};
        vPlayer += vShift;
        vRayDir += vShift; 
        camera += vShift;

        break;
    }

    case (STRAFE_RIGHT):
    {
        vf2d vShift{(vRayDir - vPlayer).perp() * XPIXEL * MOVEMENT_FACTOR * -1};
        vPlayer += vShift;
        vRayDir += vShift; 
        camera += vShift;

        break;
    }

    default:
        break;
    }
}

void Player::drawLine()
{
    float radian_angle = angle * ONE_DEGREE_RADIAN;

    camera.rotate(vPlayer, radian_angle);
    vRayDir.rotate(vPlayer, radian_angle);
    angle = 0;

    Line line{};
    line.addLine(camera.p1, camera.p2, color);
    line.addLine(vRayDir, vPlayer, color);

    // renderLines(line, 3);
}

void Player::renderLines(Line line, float size)
{

    glBindVertexArray(VAO);

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, line.vertices.size() * sizeof(GLfloat), line.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, line.indicies.size() * sizeof(GLuint), line.indicies.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glLineWidth(size);
    glDrawElements(GL_LINES, line.indicies.size(), GL_UNSIGNED_INT, 0);
    glLineWidth(1.0f);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}