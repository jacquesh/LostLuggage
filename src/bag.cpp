#include "bag.h"

Bag::Bag(dge::Vector2 _position, dge::Vector2 _size, int _category, Level* _level)
  : position(_position), size(_size), category(_category), level(_level), alongLine(1)
{
    lastPosition = dge::Vector2I(position.x,position.y);
    nextPosition = dge::Vector2I(position.x,position.y);
    velocity = dge::Vector2(0,0);
};

void Bag::updatePosition(float timestep)
{
    position = dge::lerp(lastPosition, nextPosition, alongLine);

    alongLine += timestep;

    if (alongLine > 1)
    {
        lastPosition = nextPosition;
        MapObject* currentPositionObj = level->map[nextPosition.y][nextPosition.x];
        if ((currentPositionObj != nullptr) && (currentPositionObj->type == MapObjectType::conveyer))
        {
            nextPosition += ((Conveyer*)level->map[lastPosition.y][lastPosition.x])->getSpeed();
        }
        MapObject* nextPositionObj = level->map[nextPosition.y][nextPosition.x];
        if ((nextPositionObj != nullptr) && (nextPositionObj->type == MapObjectType::wall))
        {
            nextPosition = lastPosition;
        }
        alongLine -= 1;
    }

}
