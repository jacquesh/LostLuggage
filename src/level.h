#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>

#include "dge_math.h"

enum Direction
{
  up,
  left,
  right,
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
  int **conveyerParent;
  Conveyer ***map;
  Level(int _width, int _height);
  Level(std::fstream& fin);
  ~Level();
  Vector2 getVelocity(Vector2 topLeft, Vector2 bottomRight);
  void flipConveyers(int x,int y);
};

#endif
