#include "level.h"
#include <sstream>
#include <picojson.h>
#include <math.h>

int twodfind(int** parents, int a, int b, int width)
{
    if (parents[a][b] != a*width + b)
        parents[a][b] = twodfind(parents, parents[a][b] / width, parents[a][b] % width,width);
    return parents[a][b];
}

Conveyer::Conveyer(Direction _dir)
  : dir(_dir)
{
    type = MapObjectType::conveyer;
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

Bin::Bin(int _cat)
    :  category(_cat)
{
    type = MapObjectType::bin;
}

Level::Level(int _width, int _height)
  : width(_width) , height(_height)
{
  map = new MapObject** [height];
  conveyerParent = new int* [height];
  for (int i = 0; i<height; i++)
  {
    map[i] = new MapObject* [width];
    conveyerParent[i] = new int [width];
    for (int j = 0; j<width; j++)
    {
      map[i][j] = nullptr;
      conveyerParent[i][j] = i * width + j;
    }
  }
}

Level::Level(picojson::value v)
{
    height = round(v.get("size").get<picojson::array>()[0].get<double>());
    width  = round(v.get("size").get<picojson::array>()[1].get<double>());
  map = new MapObject** [height];
  conveyerParent = new int* [height];

  for (int i = height-1; i>=0; i--)
  {
    std::stringstream sin(v.get("map").get<picojson::array>()[height-i-1].to_str());
    map[i] = new MapObject* [width];
    conveyerParent[i] = new int [width];
    for (int j = 0; j<width; j++)
    {
      map[i][j] = nullptr;
      conveyerParent[i][j] = i * width + j;
      char v;
      sin>>v;
      if((v >= 'A') && (v <= 'Z'))
      {
        int binCategory = v - 'A';
        map[i][j] = new Bin(binCategory);
      }
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
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            MapObject* obj = map[i][j];
            if((obj == nullptr) || (obj->type != MapObjectType::conveyer))
                continue;
            Conveyer* objConveyer = (Conveyer*)obj;

            for (int di = -1; di<2; di+=2)
            {
                if ((i+di > 0) && (i+di < height))
                {
                    MapObject* nextObj = map[i+di][j];
                    if((nextObj != nullptr) && (nextObj->type == MapObjectType::conveyer))
                    {
                        Conveyer* nextObjConveyer = (Conveyer*)nextObj;
                        if (objConveyer->dir == nextObjConveyer->dir)
                        {
                            conveyerParent[i][j] = twodfind(conveyerParent,i+di,j,width);
                        }
                    }
                }
            }
            for (int dj = -1; dj<2; dj+=2)
            {
                if ((j + dj > 0) && (j + dj < width))
                {
                    MapObject* nextObj = map[i][j+dj];
                    if((nextObj != nullptr) && (nextObj->type == MapObjectType::conveyer))
                    {
                        Conveyer* nextObjConveyer = (Conveyer*)nextObj;
                        if (objConveyer->dir == nextObjConveyer->dir)
                        {
                            conveyerParent[i][j] = twodfind(conveyerParent,i,j+dj,width);
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i<height; i++)
        for (int j = 0; j<width; j++)
          twodfind(conveyerParent,i,j,width);
}

void Level::flipConveyers(int x, int y)
{
    int id = conveyerParent[y][x];
    for (int i = 0; i<height; i++)
        for (int j = 0; j<width; j++)
        {
            MapObject* obj = map[i][j];
            if ((obj == nullptr) || (obj->type != MapObjectType::conveyer))
                continue;
            Conveyer* objConveyer = (Conveyer*)obj;

            if (conveyerParent[i][j] == id)
            {
                switch (objConveyer->dir)
                {
                    case up:
                        objConveyer->dir = down;
                        break;
                    case down:
                        objConveyer->dir = up;
                        break;
                    case right:
                        objConveyer->dir = left;
                        break;
                    case left:
                        objConveyer->dir = right;
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
