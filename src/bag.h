#ifndef BAG_H
#define BAG_H

#include "dge_math.h"
#include "level.h"

struct Bag
{
  Vector2 position;
  Vector2 size;
  Level* level;
  Bag(Vector2 _position, Vector2 _size, Level* _level);
  void updatePosition(float timestep);
};

#endif
