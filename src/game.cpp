#include <SDL.h>

#include <stdio.h>
#include <fstream>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "dge_math.h"
#include "dge_util.h"
#include "dge_list.h"
#include "dge_render.h"
#include "dge_debug.h"

#include "game.h"

#include "render.h"

float grid_size = 64.f;

bool handleInput(GameState* game)
{
    bool keepRunning = true;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&e);

        if (e.type == SDL_QUIT)
        {
            keepRunning = false;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            if(e.key.repeat)
            {
                continue;
            }
            switch(e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                keepRunning = false;
                break;
            }
        }
    }
    return keepRunning;
}

void initGame(GameState* game)
{
    loadRenderData();

    game->camera.position = Vector2(-grid_size, -grid_size);
    game->camera.size = Vector2(640.0f, 480.f);

    std::fstream fin("resources/level0.lvl", std::fstream::in);
    game->currentLevel = new Level(fin);
    grid_size = std::min(grid_size, 640.f / (game->currentLevel->width + 1));
    grid_size = std::min(grid_size, 480.f / (game->currentLevel->height + 1));
    int bagCount;
    int bagX;
    int bagY;
    fin >> bagCount;
    for(int i=0; i<bagCount; ++i)
    {
        fin >> bagX;
        fin >> bagY;
        Bag* bag = new Bag(Vector2(bagX, bagY), Vector2(grid_size*0.5, grid_size*0.5), game->currentLevel);
        game->bagList.insert(bag);
    }
    fin.close();
}

bool updateGame(GameState* game, float deltaTime)
{
    bool keepRunning = handleInput(game);;

    for (int i = 0; i< game->bagList.size(); ++i)
        game->bagList[i]->updatePosition(deltaTime);

    return keepRunning;
}

void cleanupGame(GameState* game)
{
}

void getWindowSettings(WindowData* windowData)
{
    windowData->x = SDL_WINDOWPOS_CENTERED;
    windowData->y = SDL_WINDOWPOS_CENTERED;
    windowData->width = 640;
    windowData->height = 480;

    windowData->flags = SDL_WINDOW_OPENGL;
    windowData->title = "Lost Luggage Clone";
}
