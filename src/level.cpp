#include "level.h"
#include <fstream>

Conveyer::Conveyer(Direction _dir)
  : dir(_dir)
{
}

Vector2I Conveyer::getSpeed()
{
  switch(dir)
  {
    case up:
      return Vector2I(0,1);
    case down:
      return Vector2I(0,-1);
    case left:
      return Vector2I(-1,0);
    case right:
      return Vector2I(1,0);
    default:
      return Vector2I(0,0);
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

Level::Level(const char* filename)
{
  std::fstream fin (filename, std::fstream::in);
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
  fin.close();
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
