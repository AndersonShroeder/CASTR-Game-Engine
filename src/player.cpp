#include "player.hh"
#include "math.h"

Player::Player(float px, float py, GLFWwindow *window)
{
    this->px = px;
    this->py = py;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
}

void Player::drawPlayer(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH])
{
    checkKeys();
    drawDot(VAO, shaderProgram);
    drawLine(VAO, shaderProgram);
    castRays(VAO, shaderProgram, map);
}

void Player::drawDot(GLuint &VAO, GLuint &shaderProgram)
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

void Player::drawLine(GLuint &VAO, GLuint &shaderProgram)
{
    float radian_angle = angle * ONE_DEGREE_RADIAN;

    // recalculate Line end point positions to account for rotation
    float rot_epx = cos(radian_angle) * (.1) - sin(radian_angle) * (0) + px;
    float rot_epy = sin(radian_angle) * (.1) + cos(radian_angle) * (0) + py;

    epx = rot_epx;
    epy = rot_epy;

    // global_angle += angle;

    // angle = 0;

    GLfloat vertices[12] =
        {
            px,
            py,
            0.0f,
            color[0],
            color[1],
            color[2], // Start point
            rot_epx,
            rot_epy,
            0.0f,
            color[0],
            color[1],
            color[2],
        };

    GLuint indicies[2] =
        {
            0, 1};

    glBindVertexArray(VAO);

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), indicies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glLineWidth(5.0f);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    glLineWidth(1.0f);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
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

        px += dpx;
        py += dpy;

        epx += dpx;
        epy += dpy;

        break;
    }

    case (BACKWARD):
    {
        float dpx = (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpy = (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px -= dpx;
        py -= dpy;

        epx -= dpx;
        epy -= dpy;

        break;
    }

    case (STRAFE_LEFT):
    {
        float dpy = (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpx = -1 * (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px += dpx;
        py += dpy;

        epx += dpx;
        epy += dpy;

        break;
    }

    case (STRAFE_RIGHT):
    {
        float dpy = -1 * (epx - px) * XPIXEL * MOVEMENT_FACTOR;
        float dpx = (epy - py) * YPIXEL * MOVEMENT_FACTOR;

        px += dpx;
        py += dpy;

        epx += dpx;
        epy += dpy;

        break;
    }

    default:
        break;
    }
}

void Player::projectVectors(int vector1[2], int vector2[2])
{

}

void Player::castRaysVertical(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH])
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indicies;
    bool up;

    // Vertical LINES

    // Slope of direction vector
    double m = (epy-py)/(epx-px);

    // Y-val of closest grid line
    
    int pixx = NORMAL_TO_PIXEL(px);
    int x_remainder =  pixx % (CELL_WIDTH);

    if (angle < 90 || angle > 270)
    {
        up = true;
        pixx = x_remainder != 0 ? (pixx - x_remainder + (CELL_WIDTH)): (pixx+(CELL_WIDTH));
    }

    else if (angle > 90 && angle < 270)
    {
        up = false;
        pixx = x_remainder != 0 ? (pixx - x_remainder): (pixx-(CELL_WIDTH));
    }

    vertices.push_back(px);
    vertices.push_back(py);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    int checked = 0;

    double xval = double(((pixx)*2))/(SCREEN_HEIGHT) - 1;
    double yval = m*(xval - px) + py;

    while (checked < MAP_HEIGHT)
    {
        int pixy = (yval + 1) * (SCREEN_HEIGHT)/2 + 1;
        int pixx = (xval + 1) * (SCREEN_WDITH)/2;

        int x_index = pixx / (CELL_WIDTH);
        int y_index = pixy / (CELL_HEIGHT) - 1;

        int index = 90 - (y_index * 10) - (10 - x_index) - 1;
        int index2 = 90 - (y_index * 10) - (10 - x_index);

        std::cout << index2 << '\n';

        if (index >= 0 && index <= 99 && index2 >= 0 && index2 <= 99)
        {
            if (map[index] != 0 || map[index2] != 0)
            {
                break;
            }
        }

        // Calculate new yval by going up one row in grid
        if (up) xval += MAP_STEP_SIZE_WIDTH;
        else xval -= MAP_STEP_SIZE_WIDTH;

        yval = m*(xval - px) + py;
        
        checked++;
    }
    
    
    vertices.push_back(xval);
    vertices.push_back(yval);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    indicies.push_back(0);
    indicies.push_back(1);
    
    renderLines(VAO, shaderProgram, indicies, vertices, 2);
}

void Player::castRaysHorizontal(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH])
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indicies;
    bool up;

    // HORIZTONTAL LINES

    // Slope of direction vector
    double m = (epy-py)/(epx-px);

    // Y-val of closest grid line
    
    int pixy = NORMAL_TO_PIXEL(py);
    int y_remainder =  pixy % (CELL_HEIGHT);

    if (angle < 180)
    {
        up = true;
        pixy = y_remainder != 0 ? (pixy - y_remainder + (CELL_HEIGHT)): (pixy+(CELL_HEIGHT));
    }

    else if (angle > 180)
    {
        up = false;
        pixy = y_remainder != 0 ? (pixy - y_remainder): (pixy-(CELL_HEIGHT));
    }

    vertices.push_back(px);
    vertices.push_back(py);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    int checked = 0;

    double yval = double(((pixy)*2))/(SCREEN_HEIGHT) - 1;
    double xval = (yval - py)/m + px;

    while (checked < MAP_HEIGHT)
    {
        int pixy = (yval + 1) * (SCREEN_HEIGHT)/2 + 1;
        int pixx = (xval + 1) * (SCREEN_WDITH)/2;

        int x_index = pixx / (CELL_WIDTH);
        int y_index = pixy / (CELL_HEIGHT) - 1;

        int index = (90 -  y_index * (MAP_HEIGHT)) + x_index;
        int index2 = 90 - (y_index * 10) - (10 - x_index);

        if (index >= 0 && index <= 99 && index2 >= 0 && index2 <= 99)
        {
            if (map[index] != 0 || map[index2] != 0)
            {
                break;
            }
        }

        // Calculate new yval by going up one row in grid
        if (up) yval += MAP_STEP_SIZE_HEIGHT;
        else yval -= MAP_STEP_SIZE_HEIGHT;

        xval = (yval - py)/m + px;
        
        checked++;
    }
    
    
    vertices.push_back(xval);
    vertices.push_back(yval);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    indicies.push_back(0);
    indicies.push_back(1);
    
    renderLines(VAO, shaderProgram, indicies, vertices, 10);
}

void Player::castRays(GLuint &VAO, GLuint &shaderProgram, int map[MAP_HEIGHT * MAP_WIDTH])
{
    castRaysHorizontal(VAO, shaderProgram, map);
    castRaysVertical(VAO, shaderProgram, map);
}

void Player::renderLines(GLuint &VAO, GLuint &shaderProgram, std::vector<GLuint> indicies, std::vector<GLfloat> vertices, float size)
{

    glBindVertexArray(VAO);

    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glLineWidth(size);
    glDrawElements(GL_LINES, indicies.size(), GL_UNSIGNED_INT, 0);
    glLineWidth(1.0f);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}