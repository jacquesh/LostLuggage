#include "level.h"

Conveyer::Conveyer(Direction _dir)
  : dir(_dir)
{
}

Level::Level(int _width, int _height)
  : width(_width) , height(_height)
{
  map = new Conveyer** [height];
  for (int i = 0; i<height; i++)
  {
    map[i] = new Conveyer* [width];
    for (int j = 0; j<width; j++)
      map[i][j] = nullptr; 
  }
}

Level::~Level()
{
  for (int i = 0; i<height; i++)
  {
    for (int j = 0; j<width; j++)
      if (map[i][j] != nullptr)
        delete map[i][j];
    delete[] map[i]; 
  }
}

Vector2 Level::getVelocity(Vector2 topLeft, Vector2 bottomRight)
{
  return Vector2(0,0);
}
