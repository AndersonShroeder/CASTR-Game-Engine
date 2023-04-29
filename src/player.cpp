#include "player.hh"
#include "math.h"

Player::Player(float px, float py, GLFWwindow *window, const GLuint &VAO, const GLuint &shaderProgram, int map[MAP_HEIGHT*MAP_WIDTH])
{
    this->vPlayer = vf2d{px, py};
    this->vRayDir = vf2d{px + .05, py};
    this->camera = CameraPlane{{px + .05, py -.05f}, {px + .05, py +.05f}};
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
    renderer.renderPoint(vPlayer, color);
    drawLine();
    DDA();
    // castRays(90);
}

void Player::DDA()
{
    // ALL VECTORS ARE IN TERMS OF GRID BLOCKS
    const vf2d vRayStart{(vPlayer.x + 1)*10/2, (vPlayer.y + 1)*10/2};
    vf2d vRayDirReal= (vf2d{(vRayDir.x + 1)*10/2, (vRayDir.y + 1)*10/2} - vRayStart).norm();
    // std::cout << vRayStart.x << " : " << vRayStart.y << '\n';
    // std::cout << vRayDirReal.x << " : " << vRayDirReal.y << '\n';


    vf2d vRayUnitStepSize = {abs(1/vRayDirReal.x), abs(1/vRayDirReal.y)};
    // std::cout << vRayUnitStepSize.y/vRayUnitStepSize.x  << '\n';

    vi2d vMapCheck{vRayStart.x, vRayStart.y};
    // std::cout << vMapCheck.x << " : " << 9 - vMapCheck.y << '\n';

    vf2d vRayLength1D;
    
    // std::cout << (vRayStart.x - (vMapCheck.x)*(MAP_STEP_SIZE_WIDTH)) * vRayUnitStepSize.x << " : " << vRayStart.y - (yIndex)*(MAP_STEP_SIZE_HEIGHT)<< '\n';

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

    // std::cout << vRayLength1D.x << " : " << vRayLength1D.y << '\n';  

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
            std::cout << (9-vMapCheck.y) * (MAP_WIDTH) + vMapCheck.x << '\n';
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

        std::cout << distance << '\n';

        Line line{};
        line.addLine(vIntersection, vPlayer, color);

        renderLines(line, 3);
    }

    
}

void Player::castRays(int FOV)
{
    std::vector<GLfloat> verticies;
    std::vector<GLuint> indicies;

    unsigned int index = 0;
    for (int i = -FOV/2; i <= FOV/2; i++)
    {
        std::vector<GLfloat> verticies1;
        std::vector<GLuint> indicies1;
        
        int newAngle = (angle + i);
        if (newAngle > 360) newAngle -= 360;
        else if (newAngle < 0) newAngle = 360 + newAngle;
        float newAngleRadian = newAngle * ((ONE_DEGREE_RADIAN));
        float rpx = cos(newAngleRadian) * (.05) - sin(newAngleRadian) * (0) + px;
        float rpy = sin(newAngleRadian) * (.05) - cos(newAngleRadian) * (0) + py;
        const float slope = (rpy-py)/(rpx-px);

        Line line1 = castRaysHorizontal(newAngle, slope);
        Line line2 = castRaysVertical(newAngle, slope);

        // Render shortest line
        (line1.length() < line2.length()) ? renderLines(line1, 5) : renderLines(line2, 5);

        // Draw 3d
        Line drawLine = (line1.length() < line2.length()) ? line1 : line2;
        float distance =  4 * drawLine.length() * cos((angle - newAngle) * ONE_DEGREE_RADIAN);
        float lineHeight = (MAP_STEP_SIZE_HEIGHT) / distance; if (lineHeight > 1){lineHeight = 1;}

        verticies.insert(verticies.end(), 
            {
                (1 - 1/(float(FOV)*2) * (index - 1)), lineHeight, 0.0f,   1.0f, 0.0f, 0.0f,
                (1 - 1/(float(FOV)*2) * (index - 1)), -lineHeight, 0.0f,  1.0f, 0.0f, 0.0f
            }
        );

        indicies.insert(indicies.end(),
            {
                index++, index++
            }
        );

        // renderLines(Line{verticies, indicies}, 20);
    }

    renderLines(Line{verticies, indicies}, 5);
}

Line Player::castRaysVertical(int newAngle, double slope)
{
    int colsChecked = 0;
    bool isLookingRight = (newAngle < 90 || newAngle > 270);
    
    int pixx = NORMAL_TO_PIXEL_X(px);
    int x_remainder =  pixx % (CELL_WIDTH);
    pixx = isLookingRight ? pixx + (CELL_WIDTH - x_remainder) : pixx - x_remainder;

    float xval = float(((pixx)*2))/(SCREEN_HEIGHT) - 1;
    float yval = slope*(xval - px) + py;

    while (colsChecked < MAP_WIDTH)
    {
        // Check for intersection
        Point point{xval, yval, VERTICAL, isLookingRight ? RIGHT : LEFT};
        if (point.intersection(this->map)) {break;}

        // Calculate new yval by going up one row in grid
        xval += isLookingRight ? MAP_STEP_SIZE_WIDTH : -MAP_STEP_SIZE_WIDTH;
        yval = slope*(xval - px) + py;
        
        colsChecked++;
    }

    std::vector<GLfloat> vertices
    {
        px, py, 0.0f, 0.0f, 1.0f, 0.0f,
        xval, yval, 0.0f, 0.0f, 1.0f, 0.0f
    };

    // Tie end points to index for rendering
    std::vector<GLuint> indicies
    {
        0, 1
    };

    return Line{vertices, indicies};
}

Line Player::castRaysHorizontal(int newAngle, double slope)
{
    int checked = 0;
    bool isLookingUp = newAngle < 180;

    int pixy = NORMAL_TO_PIXEL_Y(py);
    int y_remainder =  pixy % (CELL_HEIGHT);
    pixy = isLookingUp ? pixy - y_remainder + (CELL_HEIGHT) : (pixy - y_remainder);
    

    float yval = double(((pixy)*2))/(SCREEN_HEIGHT) - 1;
    float xval = (yval - py)/slope + px;

    while (checked < MAP_HEIGHT)
    {
        // Check for intersection with wall
        Point point{xval, yval, HORIZONTAL, isLookingUp ? UP : DOWN};
        // break;
        if (point.intersection(this->map)) {break;}

        // Calculate new yval by going up one row in grid
        yval += isLookingUp ? MAP_STEP_SIZE_HEIGHT : -MAP_STEP_SIZE_HEIGHT;
        xval = (yval - py)/slope + px;
        
        checked++;
    }
    
    // Add end points to vertices array
    std::vector<GLfloat> vertices
    {
        px, py, 0.0f, 1.0f, 0.0f, 0.0f,
        xval, yval, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // Tie end points to index for rendering
    std::vector<GLuint> indicies
    {
        0, 1
    };

    return Line{vertices, indicies};
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

    renderLines(line, 3);
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