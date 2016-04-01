#include <SDL.h>

#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "dge_math.h"
#include "dge_util.h"
#include "dge_list.h"
#include "dge_render.h"
#include "dge_debug.h"

#include "game.h"

#include "render.h"

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
}

bool updateGame(GameState* game, float deltaTime)
{
    bool keepRunning = handleInput(game);;
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
