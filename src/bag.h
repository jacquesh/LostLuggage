#ifndef BAG_H
#define BAG_H

#include "dge_math.h"
#include "level.h"

struct Bag
{
  dge::Vector2 position;
  dge::Vector2 size;
  int category;
  Level* level;
  float alongLine;
  dge::Vector2I lastPosition;
  dge::Vector2I nextPosition;
  dge::Vector2 velocity;
  Bag(dge::Vector2 _position, dge::Vector2 _size, int _category, Level* _level);
  void updatePosition(float timestep);
};

#endif
