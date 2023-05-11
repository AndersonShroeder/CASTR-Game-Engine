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

    virtual void render();

    template <class T>
    void renderGeometry(T geometry);

private:
    GLuint shaderProgram;
    GLuint VAO;
};

// class CASTRThreeDRenderer : public CASTRRenderer
// {
// };

// class CASTRRayCastRenderer : public CASTRRenderer
// {
// public:
//     int mapWidth, mapHeight;

//     CASTRRayCastRenderer(int mapWidth, int mapHeight);

//     void render() override;

//     void DDA(Lines &line, Lines &line3D, int x);
// };

class CASTRRayCastMapRenderer : public CASTRRenderer
{
public:
    CASTRRayCastMapRenderer(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY, CASTRWindow *window, std::vector<GLfloat> gridColor = {0.0f, 0.0f, 0.0f}) : CASTRRenderer(window)
    {
        createGridVertices(gridColor, stepSizeX, stepSizeY);
        createMapVertices(mapWidth, mapHeight, map, stepSizeX, stepSizeY);
    }

    void render() override;

private:
    Lines gridLines{{}, {}, {}};
    Triangles mapTriangles{{}, {}, {}};

    void drawMap();
    void drawGrid();

    void createGridVertices(std::vector<GLfloat> color, float stepSizeX, float stepSizeY);
    void createMapVertices(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY);
};