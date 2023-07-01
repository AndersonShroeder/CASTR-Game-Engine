#pragma once

#include "window.hh"
#include "entity.hh"

class CASTRRenderer
{
public:
    CASTRWindow *window = NULL;

    CASTRRenderer(CASTRWindow *window);
    ~CASTRRenderer();

    GLuint initShaderProgram();

    virtual void render();
    // virtual void renderEntity(Entity entity){};

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
    CASTRRayCastMapRenderer(CASTRWindow *window) : CASTRRenderer(window){}
    void render() override;
    void generateMap(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY, std::vector<GLfloat> gridColor = {0.0f, 0.0f, 0.0f});

private:
    Lines gridLines{{}, {}, {}};
    Triangles mapTriangles{{}, {}, {}};

    void drawMap();
    void drawGrid();

    void createGridVertices(std::vector<GLfloat> color, float stepSizeX, float stepSizeY);
    void createMapVertices(int mapWidth, int mapHeight, int *map, float stepSizeX, float stepSizeY);
};