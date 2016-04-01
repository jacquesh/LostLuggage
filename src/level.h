#ifndef LEVEL_H
#define LEVEL_H 

#include "dge_math.h"

enum Direction
{
  left,
  right,
  up,
  down
};

struct Conveyer
{
  Direction dir;
  Conveyer(Direction _dir);
};

struct Level 
{
  int width;
  int height;
  Conveyer ***map;
  Level(int _width, int _height);
  ~Level();
  Vector2 getVelocity(Vector2 topLeft, Vector2 bottomRight);
};

#endif
