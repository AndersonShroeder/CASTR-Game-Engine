#pragma once
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "config.hh"

template <class T>
struct v2d_generic
{
    T x = 0;
    T y = 0;

    inline v2d_generic() : x(0), y(0) {}
    inline v2d_generic(T _x, T _y) : x(_x), y(_y) {}
    inline v2d_generic(const v2d_generic &v) : x(v.x), y(v.y) {}
    inline T mag() { return sqrt(x * x + y * y); }
    inline v2d_generic norm() {T r = (1) / mag(); return v2d_generic(x * r, y * r);}
    inline v2d_generic perp() { return v2d_generic(-y, x); }
    inline T dot(const v2d_generic &rhs) { return this->x * rhs.x + this->y * rhs.y; }
    inline T cross(const v2d_generic &rhs) { return this->x * rhs.y - this->y * rhs.x; }
    inline v2d_generic operator+(const v2d_generic &rhs) const { return v2d_generic(this->x + rhs.x, this->y + rhs.y); }
    inline v2d_generic operator-(const v2d_generic &rhs) const { return v2d_generic(this->x - rhs.x, this->y - rhs.y); }
    inline v2d_generic operator*(const T &rhs) const { return v2d_generic(this->x * rhs, this->y * rhs); }
    inline v2d_generic operator/(const T &rhs) const { return v2d_generic(this->x / rhs, this->y / rhs); }
    inline v2d_generic &operator+=(const v2d_generic &rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
    inline v2d_generic &operator-=(const v2d_generic &rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
    inline v2d_generic &operator*=(const T &rhs) { this->x *= rhs; this->y *= rhs; return *this; }
    inline v2d_generic &operator/=(const T &rhs) { this->x /= rhs; this->y /= rhs; return *this; }
    inline T &operator[](std::size_t i) { return *((T *)this + i); /* <-- D'oh :( */ }
    void rotate(v2d_generic vOrigin, float angle)
    {
        float x1 = cos(angle) * (x - vOrigin.x) - sin(angle) * (y - vOrigin.y) + vOrigin.x;
        float y1 = sin(angle) * (x - vOrigin.x) + cos(angle) * (y - vOrigin.y) + vOrigin.y;
        this->x = x1;
        this->y = y1;
    }
};

typedef v2d_generic<int> vi2d;
typedef v2d_generic<float> vf2d;
typedef v2d_generic<double> vd2d;

class Geometry
{
public:
    float size;
    int type;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indicies;

    Geometry(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size);

    virtual void setSize(){};
};

class Lines : public Geometry
{
public:
    Lines(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size = 1.0f);

    void setSize() override;

    template <class T1, class T2>
    void addLine(T1 p1, T2 p2, GLfloat color[3]);

    float length();
};

class Triangles : public Geometry
{
public:
    Triangles(std::vector<GLfloat> vertices, std::vector<GLuint> indicies, float size = 1.0f);

    void setSize() override{}
};

class Point : public Geometry
{
public:
    Point(vf2d v, std::vector<GLfloat> color, float size);

    void setSize() override
    {
        glPointSize(size);
    }
};