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
    type = MapObjectType::CONVEYER;
}

dge::Vector2I Conveyer::getSpeed()
{
    switch(dir)
    {
        case UP:
            return dge::Vector2I(0,1);
        case DOWN:
            return dge::Vector2I(0,-1);
        case LEFT:
            return dge::Vector2I(-1,0);
        case RIGHT:
            return dge::Vector2I(1,0);
        default:
            return dge::Vector2I(0,0);
    }
}

Wall::Wall(Direction _dir)
    : dir(_dir)
{
    type = MapObjectType::WALL;
}

Bin::Bin(int _cat)
    :  category(_cat)
{
    type = MapObjectType::BIN;
}

Bumper::Bumper(Direction _dir)
    : dir(_dir)
{
    type = MapObjectType::BUMPER;
    active = true;
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
    height = dge::round(v.get("size").get<picojson::array>()[0].get<double>());
    width  = dge::round(v.get("size").get<picojson::array>()[1].get<double>());
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
                    map[i][j] = new Conveyer(UP);
                    break;
                case '>':
                    map[i][j] = new Conveyer(RIGHT);
                    break;
                case '<':
                    map[i][j] = new Conveyer(LEFT);
                    break;
                case 'v':
                    map[i][j] = new Conveyer(DOWN);
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
            if((obj == nullptr) || (obj->type != MapObjectType::CONVEYER))
                continue;
            Conveyer* objConveyer = (Conveyer*)obj;

            for (int di = -1; di<2; di+=2)
            {
                if ((i+di > 0) && (i+di < height))
                {
                    MapObject* nextObj = map[i+di][j];
                    if((nextObj != nullptr) && (nextObj->type == MapObjectType::CONVEYER))
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
                    if((nextObj != nullptr) && (nextObj->type == MapObjectType::CONVEYER))
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
    for(picojson::array::iterator it = v.get("walls").get<picojson::array>().begin();
            it != v.get("walls").get<picojson::array>().end();
            ++it)
    {
        int x = int((*it).get<picojson::array>()[0].get<double>());
        int y = int((*it).get<picojson::array>()[1].get<double>());
        int d = (*it).get<picojson::array>()[2].to_str()[0];
        Direction dir;
        switch (d)
        {
            case 'u': dir = UP; break;
            case 'd': dir = DOWN; break;
            case 'l': dir = LEFT; break;
            default: dir = RIGHT; break;
        }
        map[y-1][x-1] = new Wall(dir);
    }

    for(picojson::array::iterator it = v.get("bumpers").get<picojson::array>().begin();
            it != v.get("bumpers").get<picojson::array>().end();
            ++it)
    {
        int x = int((*it).get<picojson::array>()[0].get<double>());
        int y = int((*it).get<picojson::array>()[1].get<double>());
        char d = (*it).get<picojson::array>()[2].to_str()[0];
        Direction dir;
        switch(d)
        {
            case 'u': dir = UP; break;
            case 'd': dir = DOWN; break;
            case 'l': dir = DOWN; break;
            default: dir = RIGHT; break;
        }
        map[y-1][x-1] = new Bumper(dir);
    }
}

void Level::flipConveyers(int x, int y)
{
    int id = conveyerParent[y][x];
    for (int i = 0; i<height; i++)
        for (int j = 0; j<width; j++)
        {
            MapObject* obj = map[i][j];
            if ((obj == nullptr) || (obj->type != MapObjectType::CONVEYER))
                continue;
            Conveyer* objConveyer = (Conveyer*)obj;

            if (conveyerParent[i][j] == id)
            {
                switch (objConveyer->dir)
                {
                    case UP:
                        objConveyer->dir = DOWN;
                        break;
                    case DOWN:
                        objConveyer->dir = UP;
                        break;
                    case RIGHT:
                        objConveyer->dir = LEFT;
                        break;
                    case LEFT:
                        objConveyer->dir = RIGHT;
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
