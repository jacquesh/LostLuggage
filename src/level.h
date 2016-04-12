#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>

#include <picojson.h>

#include "dge_math.h"


enum Direction
{
    UP,
    LEFT,
    RIGHT,
    DOWN
};

enum MapObjectType
{
    CONVEYER,
    BIN,
    WALL,
    BUMPER
};

struct MapObject
{
    MapObjectType type;
};

struct Conveyer : MapObject
{
    Direction dir;
    Conveyer(Direction _dir);
    dge::Vector2I getSpeed();
};

struct Bin : MapObject
{
    int category;
    Bin(int _cat);
};

struct Wall : MapObject
{
    Direction dir;
    Wall(Direction _dir);
};

struct Bumper : MapObject
{
    Direction dir;
    bool active;
    Bumper(Direction _dir);
};

struct Level
{
    int width;
    int height;
    int **conveyerParent;
    MapObject ***map;
    Level(int _width, int _height);
    Level(picojson::value v);
    ~Level();
    dge::Vector2 getVelocity(dge::Vector2 topLeft, dge::Vector2 bottomRight);
    void flipConveyers(int x,int y);
};

#endif
