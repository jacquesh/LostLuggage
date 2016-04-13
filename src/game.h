#ifndef GAME_H
#define GAME_H

#include "dge_util.h"
#include "dge_render.h"
#include "dge_math.h"
#include "dge_list.h"

#include "level.h"
#include "bag.h"

#include <string>

extern float grid_size;

struct GameState
{
    dge::CameraState camera;

    dge::UnorderedList<std::string> levelFileList;
    int currentLevelIndex;
    Level* currentLevel;
    dge::UnorderedList<Bag*> bagList;

    float timeTillLevelLoad;
};

#endif
