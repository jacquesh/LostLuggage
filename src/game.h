#ifndef GAME_H
#define GAME_H

#include "dge_util.h"
#include "dge_render.h"
#include "dge_math.h"
#include "dge_list.h"

#include "level.h"
#include "bag.h"

extern float grid_size;

struct GameState
{
    dge::CameraState camera;

    Level* currentLevel;
    dge::UnorderedList<Bag*> bagList;

    float timeTillLevelRestart;
};

#endif
