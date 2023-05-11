#include "geometry.hh"
Geometry::Geometry(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size)
{
    this->vertices = vertices;
    this->indicies = indicies;
    this->size = size;
}

Lines::Lines(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size = 1.0f) : Geometry(vertices, indicies, size)
{
    this->type = GL_LINES;
}

void Lines::setSize()
{
    glLineWidth(size);
}

template <class T1, class T2>
void Lines::addLine(T1 p1, T2 p2, GLfloat color[3])
{
    unsigned int indexOne = indicies.size();
    unsigned int indexTwo = indicies.size() + 1;

    vertices.insert(
        vertices.end(),
        {
            p1.x, p1.y, 0.0f, color[0], color[1], color[2],
            p2.x, p2.y, 0.0f, color[0], color[1], color[2],
        }
    );

    indicies.insert(
        indicies.end(),
        {
            indexOne, indexTwo,
        }
    );
}

float Lines::length()
{
    return sqrt(pow((vertices.at(0) - vertices.at(6)), 2) + pow((vertices.at(1) - vertices.at(7)), 2));
}

Triangles::Triangles(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size = 1.0f) : Geometry(vertices, indicies, size)
{
    this->type = GL_TRIANGLES;
}

Point::Point(vf2d v, std::vector<GLfloat> color, float size) : Geometry({v.x, v.y, 0.0f, color[0], color[1], color[2]}, {0}, size)
{
    this->type = GL_POINTS;
}