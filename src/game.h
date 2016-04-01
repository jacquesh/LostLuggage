#ifndef GAME_H
#define GAME_H

#include "dge_util.h"
#include "dge_render.h"
#include "dge_math.h"
#include "dge_list.h"

#include "level.h"
#include "bag.h"

#define GRID_SIZE 64.0f

struct GameState
{
    CameraState camera;

    Level* currentLevel;
    UnorderedList<Bag*> bagList;
};

#endif
