#include "bag.h"

Bag::Bag(Vector2 _position, Vector2 _size, Level* _level)
  : position(_position), size(_size), level(_level)
{
};

void Bag::updatePosition(float timestep)
{
    Vector2 velocity = level->getVelocity(position - 0.5 * size, position + 0.5 * size);
    position += timestep * velocity;
}
