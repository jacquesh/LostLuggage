#include "bag.h"

Bag::Bag(Vector2 _position, Vector2 _size, Level* _level)
  : position(_position), size(_size), level(_level), alongLine(1)
{
    lastPosition = Vector2I(position.x,position.y);
    nextPosition = Vector2I(position.x,position.y);
    velocity = Vector2(0,0);
};

void Bag::updatePosition(float timestep)
{
    position = lerp(lastPosition, nextPosition, alongLine);

    alongLine += timestep;

    if (alongLine > 1)
    {
        if (level->map[nextPosition.y][nextPosition.x] != nullptr)
        {
            lastPosition = nextPosition;
            nextPosition += level->map[lastPosition.y][lastPosition.x]->getSpeed();
        }
        alongLine -= 1;
    }

}
