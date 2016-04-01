#ifndef BAG_H
#define BAG_H

#include "dge_math.h"

struct Bag
{
  Vector2 position;
  Vector2 size;
  Bag(Vector2 _position, Vector2 _size);
  void updatePosition(float timestep);
};

#endif
