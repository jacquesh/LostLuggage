#include "level.h"
#include <fstream>

Conveyer::Conveyer(Direction _dir)
  : dir(_dir)
{
}

Vector2 Conveyer::getSpeed()
{
  switch(dir)
  {
    case up:
      return Vector2(0,1);
    case down:
      return Vector2(0,-1);
    case left:
      return Vector2(-1,0);
    case right:
      return Vector2(1,0);
    default:
      return Vector2(0,0);
  }
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

Level::Level(std::fstream& fin)
{
  fin>>height>>width;
  map = new Conveyer** [height];
  for (int i = height-1; i>=0; i--)
  {
    map[i] = new Conveyer* [width];
    for (int j = 0; j<width; j++)
    {
      map[i][j] = nullptr;
      char v;
      fin>>v;
      switch (v)
      {
        case '.':
          break;
        case '^':
          map[i][j] = new Conveyer(up);
          break;
        case '>':
          map[i][j] = new Conveyer(right);
          break;
        case '<':
          map[i][j] = new Conveyer(left);
          break;
        case 'v':
          map[i][j] = new Conveyer(down);
          break;
        default:
          break;
      }
    }
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
  Vector2 ans = Vector2(0,0);
  for (int x = int(topLeft.x+0.5); x <= int(bottomRight.x+0.5-0.0001); ++x)
    for (int y = int(topLeft.y+0.5); y >= int(bottomRight.y+0.5-0.0001); --y)
      if (map[y][x] != nullptr)
          ans += map[y][x]->getSpeed();
  return ans.normalized();
}
