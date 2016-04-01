#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>

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
  Vector2I getSpeed();
};

struct Level
{
  int width;
  int height;
  Conveyer ***map;
  Level(int _width, int _height);
  Level(std::fstream& fin);
  ~Level();
  Vector2 getVelocity(Vector2 topLeft, Vector2 bottomRight);
};

#endif
