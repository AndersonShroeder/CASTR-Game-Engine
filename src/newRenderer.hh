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
    void renderGeometry(T geometry)
    {
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        GLuint gridVBO, gridIBO;
        glGenBuffers(1, &gridVBO);
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(GLfloat), geometry.vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &gridIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indicies.size() * sizeof(GLuint), geometry.indicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

        geometry.setSize();
        glDrawElements(geometry.type, geometry.indicies.size(), GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &gridVBO);
        glDeleteBuffers(1, &gridIBO);

        glBindVertexArray(0);
    }

private:
    GLuint shaderProgram;
    GLuint VAO;
};

// class CASTRThreeDRenderer : public CASTRRenderer
// {
// };

class CASTRRayCastRenderer : public CASTRRenderer
{
public:
    int mapWidth, mapHeight;

    CASTRRayCastRenderer(int mapWidth, int mapHeight);

    void render() override;

    void DDA(Lines &line, Lines &line3D, int x);
};

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