#ifndef _RENDER_H
#define _RENDER_H

#include "game.h"
#include "dge_math.h"

void loadRenderData();
void renderGame(GameState* game);
void lateRender(GameState* game);

#endif
