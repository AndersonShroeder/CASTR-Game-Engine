#include "player.hh"
#include "math.h"

Player::Player(float px, float py, GLFWwindow* window)
{
    this->px = px;
    this->py = py;

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
}

void Player::drawPlayer(GLuint& VAO, GLuint &shaderProgram)
{
    checkKeys();
    drawDot(VAO, shaderProgram);
    drawLine(VAO, shaderProgram);
}

void Player::drawDot(GLuint& VAO, GLuint &shaderProgram)
{
    GLfloat vertices[6] = {px, py, 0.0f, color[0], color[1], color[2]};
    
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, 1);
    glPointSize(1.0f);

    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

void Player::drawLine(GLuint& VAO, GLuint &shaderProgram)
{
    float radian_angle = angle*ONE_DEGREE_RADIAN;

    // recalculate Line end point positions to account for rotation
    float rot_epx = cos(radian_angle) * (epx-px) - sin(radian_angle) * (epy - py) + px;
    float rot_epy = sin(radian_angle) * (epx-px) + cos(radian_angle) * (epy - py) + py;

    epx = rot_epx;
    epy = rot_epy;

    angle = 0;

    GLfloat vertices[12] = 
    {
        px, py, 0.0f, color[0], color[1], color[2],  // Start point
        rot_epx, rot_epy, 0.0f, color[0], color[1], color[2],
    };

    GLuint indicies[2] =
    {
        0, 1
    };

    glBindVertexArray(VAO);
    
    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), indicies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glLineWidth(5.0f);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    glLineWidth(1.0f);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}

void Player::checkKeys()
{
    if (keys.e_key) angle = angle <= -360 ? angle + 360 - ONE_DEGREE * ROTATION_FACTOR : angle - ONE_DEGREE * ROTATION_FACTOR;
    if (keys.q_key) angle = (angle + ONE_DEGREE * ROTATION_FACTOR) % 360;
    if (keys.w_key) translate(FORWARD);
    if (keys.s_key) translate(BACKWARD);
    if (keys.a_key) translate(STRAFE_LEFT);
    if (keys.d_key) translate(STRAFE_RIGHT);
}

void Player::translate(Directions direction)
{
    switch (direction)
    {
        case (FORWARD):
        {
            float dpx = (epx - px)*XPIXEL * MOVEMENT_FACTOR;
            float dpy = (epy - py)*YPIXEL * MOVEMENT_FACTOR;

            px += dpx;
            py += dpy;

            epx += dpx;
            epy += dpy;

            break;
        }

        case (BACKWARD):
        {
            float dpx = (epx - px)*XPIXEL * MOVEMENT_FACTOR;
            float dpy = (epy - py)*YPIXEL * MOVEMENT_FACTOR;

            px -= dpx;
            py -= dpy;

            epx -= dpx;
            epy -= dpy;

            break;
        }

        case (STRAFE_LEFT):
        {
            float dpy = (epx - px)*XPIXEL * MOVEMENT_FACTOR;
            float dpx = -1*(epy - py)*YPIXEL * MOVEMENT_FACTOR;

            px += dpx;
            py += dpy;

            epx += dpx;
            epy += dpy;

            break;
        }

        case (STRAFE_RIGHT):
        {
            float dpy = -1*(epx - px) * XPIXEL * MOVEMENT_FACTOR;
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