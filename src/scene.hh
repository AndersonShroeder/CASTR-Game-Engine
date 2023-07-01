#pragma once
#include "newRenderer.hh"

class CASTRScene
{
public:
    CASTRRenderer* sceneRenderer;
    std::vector<Entity> entities;
    CASTRScene(CASTRRenderer* sceneRenderer)
    {
        this->sceneRenderer = sceneRenderer;
    }

    void addEntity(Entity entity);
};

class CASTRRayCastScene : public CASTRScene
{
public:
    CASTRRayCastScene(CASTRRayCastMapRenderer* sceneRenderer) : CASTRScene(sceneRenderer){} 
};