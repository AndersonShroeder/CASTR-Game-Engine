#pragma once

#include <math.h>
#include "window.hh"
#include "geometry.hh"

class CASTRRenderer
{
public:
    CASTRWindow *window = NULL;

    CASTRRenderer(CASTRWindow *window);
    ~CASTRRenderer();

    GLuint initShaderProgram();

    void render();

    template <class T>
    void renderGeometry(T geometry);

private:
    GLuint shaderProgram;
    GLuint VAO;
};

class CASTRThreeDRenderer : public CASTRRenderer
{
};

class CASTRRayCastRenderer : public CASTRRenderer
{
public:
    int mapWidth, mapHeight;

    CASTRRayCastRenderer(int mapWidth, int mapHeight);

    void render();

    void DDA(Lines &line, Lines &line3D, int x);
};

class CASTRRayCastMapRenderer : public CASTRRenderer
{
public:
    Lines gridLines{{}, {}, {}};
    Triangles mapTriangles{{}, {}, {}};
    CASTRRayCastMapRenderer(int mapWidth, int mapHeight, int *map, CASTRWindow* window, std::vector<GLfloat> color = {0.0f, 0.0f, 0.0f}) : CASTRRenderer(window)
    {
    createGridVertices(color, 1.0/5.0);
    createMapVertices(mapWidth, mapHeight, color);
    }

    void createGridVertices(std::vector<GLfloat> color, float stepSize);
    void createMapVertices(int mapWidth, int mapHeight, std::vector<GLfloat> color);

    void render();

    void drawMap(int map[]);
    void drawGrid();

private:
};