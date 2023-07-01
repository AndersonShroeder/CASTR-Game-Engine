#include "geometry.hh"

class Entity
{
public:
    vf2d vPos;

    void updatePos(float x, float y)
    {
        vPos.x += x;
        vPos.y += y;
    }
};
