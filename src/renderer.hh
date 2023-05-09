// #pragma once

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include "config.hh"
// #include <math.h>

// template <class T>
// struct v2d_generic
// {
//     T x = 0;
//     T y = 0;

//     inline v2d_generic() : x(0), y(0) {}
//     inline v2d_generic(T _x, T _y) : x(_x), y(_y) {}
//     inline v2d_generic(const v2d_generic &v) : x(v.x), y(v.y) {}
//     inline T mag() { return sqrt(x * x + y * y); }
//     inline v2d_generic norm()
//     {
//         T r = (1) / mag();
//         return v2d_generic(x * r, y * r);
//     }
//     inline v2d_generic perp() { return v2d_generic(-y, x); }
//     inline T dot(const v2d_generic &rhs) { return this->x * rhs.x + this->y * rhs.y; }
//     inline T cross(const v2d_generic &rhs) { return this->x * rhs.y - this->y * rhs.x; }
//     inline v2d_generic operator+(const v2d_generic &rhs) const { return v2d_generic(this->x + rhs.x, this->y + rhs.y); }
//     inline v2d_generic operator-(const v2d_generic &rhs) const { return v2d_generic(this->x - rhs.x, this->y - rhs.y); }
//     inline v2d_generic operator*(const T &rhs) const { return v2d_generic(this->x * rhs, this->y * rhs); }
//     inline v2d_generic operator/(const T &rhs) const { return v2d_generic(this->x / rhs, this->y / rhs); }
//     inline v2d_generic &operator+=(const v2d_generic &rhs)
//     {
//         this->x += rhs.x;
//         this->y += rhs.y;
//         return *this;
//     }
//     inline v2d_generic &operator-=(const v2d_generic &rhs)
//     {
//         this->x -= rhs.x;
//         this->y -= rhs.y;
//         return *this;
//     }
//     inline v2d_generic &operator*=(const T &rhs)
//     {
//         this->x *= rhs;
//         this->y *= rhs;
//         return *this;
//     }
//     inline v2d_generic &operator/=(const T &rhs)
//     {
//         this->x /= rhs;
//         this->y /= rhs;
//         return *this;
//     }
//     inline T &operator[](std::size_t i) { return *((T *)this + i); /* <-- D'oh :( */ }

//     void rotate(v2d_generic vOrigin, float angle)
//     {
//         float x1 = cos(angle) * (x - vOrigin.x) - sin(angle) * (y - vOrigin.y) + vOrigin.x;
//         float y1 = sin(angle) * (x - vOrigin.x) + cos(angle) * (y - vOrigin.y) + vOrigin.y;

//         this->x = x1;
//         this->y = y1;
//     }
// };

// typedef v2d_generic<int> vi2d;
// typedef v2d_generic<float> vf2d;
// typedef v2d_generic<double> vd2d;

// struct Line
// {
//     // Verticies is a vector of length 12 containing two points
//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indicies;
//     std::vector<GLfloat> color;

//     template<class T1, class T2>
//     void addLine(T1 p1, T2 p2, float color[3])
//     {
//         vertices.insert(
//             vertices.end(),
//             {
//                 p1.x, p1.y, 0.0f,   color[0], color[1], color[2],
//                 p2.x, p2.y, 0.0f,   color[0], color[1], color[2],
//             }
//         );

//         unsigned int indexOne = indicies.size();
//         unsigned int indexTwo = indicies.size() + 1;
//         indicies.insert(
//             indicies.end(),
//             {
//                 indexOne,
//                 indexTwo,
//             }
//         );

//     }
    

//     float length()
//     {
//         return sqrt(pow((vertices.at(0) - vertices.at(6)), 2) + pow((vertices.at(1) - vertices.at(7)), 2));
//     }
// };

// class Renderer
// {
// public:
//     GLFWwindow *window;
//     GLuint shaderProgram;
//     GLuint VAO;

//     bool generatedMap = false;
//     GLfloat gridVertices[((MAP_HEIGHT - 2) + (MAP_WIDTH - 2)) * 6 * 2]; // There are (n-2) + (m-2) lines in a grid, 6 attributes per vertex and 2 vertices per line
//     GLfloat gridIndices[((MAP_HEIGHT - 2) + (MAP_WIDTH - 2)) * 2];      // There are (n-2) + (m-2) lines in a grid, 2 vertices per line

//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;

//     Renderer();
//     ~Renderer();

//     void renderPoint(vf2d point, float color[3]);

//     void drawMap(int map[]);
//     void drawGrid();

// private:
//     GLFWwindow *initWindow();
//     GLuint initShader();
//     void createGridVertices(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, GLfloat color[3]);
// };