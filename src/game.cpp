#define NOMINMAX
#include <SDL.h>

#include <stdio.h>
#include <fstream>
#include <algorithm>

#include <picojson.h>

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
        switch (e.type)
        {
            case SDL_QUIT:
            {
                keepRunning = false;
            } break;
            case SDL_KEYDOWN:
            {
                if(e.key.repeat)
                {
                    continue;
                }
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        keepRunning = false;
                        break;
                }
            } break;
            case  SDL_MOUSEBUTTONDOWN:
            {
                game->currentLevel->flipConveyers(int(e.motion.x/grid_size)-1,int((480-e.motion.y)/grid_size)-1);
            } break;
        }
    }
    return keepRunning;
}

void loadLevel(GameState* game, const char* filename)
{
    if(game->currentLevel)
    {
        delete game->currentLevel;
        game->bagList.pointerClear();
    }

    game->camera.position = Vector2(-grid_size, -grid_size);
    game->camera.size = Vector2(640.0f, 480.f);

    std::fstream fin(filename, std::fstream::in);
    picojson::value v;
    std::string err;
    std::istream_iterator<char> input(fin);
    input = picojson::parse(v,input, std::istream_iterator<char>(),&err);
    if (! err.empty()) debug("%s",err.c_str());

    game->currentLevel = new Level(v);

    grid_size = std::min(grid_size, 640.f / (game->currentLevel->width + 1));
    grid_size = std::min(grid_size, 480.f / (game->currentLevel->height + 1));
    int bagCount;
    int bagX;
    int bagY;
    char bagCategory;
    Vector2 bagSize(grid_size*0.5f, grid_size*0.5f);
    picojson::array luggage = v.get("luggage").get<picojson::array>();
    for(picojson::array::iterator it = luggage.begin();
                                  it != luggage.end();
                                  ++it)
    {
        bagX = round((*it).get<picojson::array>()[0].get<double>());
        bagY = round((*it).get<picojson::array>()[1].get<double>());
        bagCategory = (*it).get<picojson::array>()[2].to_str()[0];
        Bag* bag = new Bag(Vector2(bagX, bagY), bagSize, game->currentLevel);
        bag->category = bagCategory - 'A';
        game->bagList.insert(bag);
    }
    fin.close();
}

void initGame(GameState* game)
{
    loadRenderData();

    game->camera.position = Vector2(-grid_size, -grid_size);
    game->camera.size = Vector2(640.0f, 480.f);

    loadLevel(game, "resources/level0.json");
}

bool updateGame(GameState* game, float deltaTime)
{
    bool keepRunning = handleInput(game);;

    int activeBagCount = game->bagList.size();
    for (int i = 0; i< game->bagList.size(); ++i)
    {
        game->bagList[i]->updatePosition(deltaTime);
        Vector2I lastLoc = game->bagList[i]->lastPosition;
        MapObject* lastLocObj = game->currentLevel->map[lastLoc.y][lastLoc.x];
        if(!lastLocObj || (lastLocObj->type == MapObjectType::bin))
        {
            activeBagCount -= 1;
        }
    }

    if(activeBagCount <= 0)
    {
        loadLevel(game, "resources/level0.json");
        // TODO: Game over (did we win? Nobody knows...its a MYSTERY!)
    }

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
