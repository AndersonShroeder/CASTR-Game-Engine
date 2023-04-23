#include "player.hh"
#include "math.h"

Player::Player(float px, float py, GLFWwindow *window, const GLuint &VAO, const GLuint &shaderProgram, int map[MAP_HEIGHT*MAP_WIDTH])
{
    this->px = px;
    this->py = py;
    this->VAO = VAO;
    this->shaderProgram = shaderProgram;
    this->map = map;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
}

void Player::drawPlayer()
{
    checkKeys();
    drawDot();
    drawLine();
    castRays(60);
}

void Player::castRays(int FOV)
{
    std::vector<GLfloat> verticies;
    std::vector<GLuint> indicies;

    unsigned int index = 0;
    for (int i = -FOV/2; i <= FOV/2; i++)
    {
        int newAngle = (angle + i);
        if (newAngle > 360) newAngle -= 360;
        else if (newAngle < 0) newAngle = 360 + newAngle;
        float newAngleRadian = newAngle * (ONE_DEGREE_RADIAN);
        float rpx = cos(newAngleRadian) * (.1) - sin(newAngleRadian) * (0) + px;
        float rpy = sin(newAngleRadian) * (.1) - cos(newAngleRadian) * (0) + py;
        const float slope = (rpy-py)/(rpx-px);

        Line line1 = castRaysHorizontal(newAngle, slope);
        Line line2 = castRaysVertical(newAngle, slope);

        // Render shortest line
        (line1.length() < line2.length()) ? renderLines(line1, 5) : renderLines(line2, 5);

        // Draw 3d
        Line drawLine = (line1.length() < line2.length()) ? line1 : line2;
        float lineHeight = (MAP_STEP_SIZE_HEIGHT) / drawLine.length(); if (lineHeight > 1){lineHeight = 1;}

        verticies.insert(verticies.end(), 
            {
                (2/float(FOV) * index) + .5f, -1, 0.0f,                1.0f, 0.0f, 0.0f,
                (2/float(FOV) * (index + 1)) + .5f, lineHeight, 0.0f,   1.0f, 0.0f, 0.0f
            }
        );

        indicies.insert(indicies.end(),
            {
                index++, index++
            }
        );

        renderLines(Line{verticies, indicies}, 800/FOV);
    }
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
        Point point{xval, yval, VERTICAL};
        // break;
        if (point.intersection(this->map)) break;

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
        Point point{xval, yval, HORIZONTAL};
        // break;
        if (point.intersection(this->map)) break;

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
        float dpx = (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpy = (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px += dpx; py += dpy;
        epx += dpx; epy += dpy;

        break;
    }

    case (BACKWARD):
    {
        float dpx = (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpy = (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px -= dpx; py -= dpy;
        epx -= dpx; epy -= dpy;

        break;
    }

    case (STRAFE_LEFT):
    {
        float dpy = (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpx = -1 * (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px += dpx; py += dpy;
        epx += dpx; epy += dpy;

        break;
    }

    case (STRAFE_RIGHT):
    {
        float dpy = -1 * (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpx = (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px += dpx; py += dpy;
        epx += dpx; epy += dpy;

        break;
    }

    default:
        break;
    }
}

void Player::drawDot()
{
    GLfloat vertices[6] = {px, py, 0.0f, color[0], color[1], color[2]};

    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, 1);
    glPointSize(1.0f);

    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

void Player::drawLine()
{
    float radian_angle = angle * ONE_DEGREE_RADIAN;

    // recalculate Line end point positions to account for rotation
    float rot_epx = cos(radian_angle) * (.1) - sin(radian_angle) * (0) + px;
    float rot_epy = sin(radian_angle) * (.1) + cos(radian_angle) * (0) + py;

    epx = rot_epx;
    epy = rot_epy;

    std::vector<GLfloat> vertices =
    {
        px, py, 0.0f,           color[0], color[1], color[2], // Start point
        rot_epx, rot_epy, 0.0f, color[0], color[1], color[2],
    };

    std::vector<GLuint> indicies
    {
        0, 1
    };

    renderLines(Line{vertices, indicies}, 3);
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