#ifndef GAME_H
#define GAME_H

#include "dge_util.h"
#include "dge_render.h"
#include "dge_math.h"
#include "dge_list.h"


#define MAP_FULL    0x01
#define MAP_LEFT    0x02
#define MAP_RIGHT   0x04
#define MAP_UP      0x08
#define MAP_DOWN    0x10
#define MAP_TOWER   0x20

#define GRID_SIZE 48.0f

struct EnemyState
{
    int32 health;

    Vector2 position;
    float rotation;
    float moveSpeed;

    Vector2I* waypoints; // TODO: Destructor!
    int waypointCount;
    int currentWaypoint;
};

struct TowerType
{
    int initialStock;

    Vector2I* attackCells; // TODO: Destructor!
    int attackCellCount;

    Vector2I* buffCells; // TODO: Destructor!
    int buffCellCount;
};

struct TowerState
{
    Vector2I position;
    int type;
    int damage;
};

struct DamageTextParticle
{
    Vector2 position;
    float age;

    char text[2];
};

enum GameScene
{
    SCENE_MENU,
    SCENE_INGAME
};

struct GameState
{
    GameScene currentScene;

    CameraState camera;
    int mapWidth;
    int mapHeight;
    uint8* map;

    bool runInProgress;

    UnorderedList<EnemyState*> enemyList;
    UnorderedList<TowerState*> towerList;
    UnorderedList<DamageTextParticle*> damageTextList;

    UnorderedList<TowerType*> towerTypeList;
    int* towerTypeStock;

    Vector2I cursorTile;
    int selectedTowerType;

    int currentLevel;
    float levelChangeCoundown;
};

#endif
