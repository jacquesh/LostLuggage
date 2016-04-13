#define NOMINMAX
#include <SDL.h>

#include <stdio.h>
#include <string.h>
#include <dirent.h>
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
                int gridX = dge::round((e.button.x + game->camera.position.x)/grid_size);
                int gridY = dge::round((480 - e.button.y + game->camera.position.y)/grid_size);
                game->currentLevel->flipConveyers(gridX, gridY);
            } break;
        }
    }
    return keepRunning;
}

void loadCurrentLevel(GameState* game)
{
    if(game->currentLevel)
    {
        delete game->currentLevel;
        game->bagList.pointerClear();
    }

    const char* filename = game->levelFileList[game->currentLevelIndex].c_str();
    std::fstream fin(filename, std::fstream::in);
    picojson::value v;
    std::string err;
    std::istream_iterator<char> input(fin);
    input = picojson::parse(v,input, std::istream_iterator<char>(),&err);
    if (! err.empty()) debug("%s",err.c_str());

    game->currentLevel = new Level(v);

    grid_size = std::min(grid_size, 640.f / (game->currentLevel->width + 1));
    grid_size = std::min(grid_size, 480.f / (game->currentLevel->height + 1));
    int bagX;
    int bagY;
    char bagCategory;
    dge::Vector2 bagSize(0.5f, 0.5f);
    picojson::array luggage = v.get("luggage").get<picojson::array>();
    for(picojson::array::iterator it = luggage.begin();
                                  it != luggage.end();
                                  ++it)
    {
        bagX = dge::round((*it).get<picojson::array>()[0].get<double>());
        bagY = dge::round((*it).get<picojson::array>()[1].get<double>());
        bagCategory = (*it).get<picojson::array>()[2].to_str()[0];
        Bag* bag = new Bag(dge::Vector2(bagX, bagY), bagSize, bagCategory - 'A', game->currentLevel);
        game->bagList.insert(bag);
    }
    fin.close();
}

void initGame(GameState* game)
{
    loadRenderData();

    game->camera.position = dge::Vector2(-grid_size, -grid_size);
    game->camera.size = dge::Vector2(640.0f, 480.f);
    game->timeTillLevelLoad = 0.0f;

    std::string resourceDir = "resources/levels/";
    DIR* levelDir = opendir(resourceDir.c_str());
    if(levelDir)
    {
        dirent* entry;
        while(entry = readdir(levelDir))
        {
            if(entry->d_type != DT_REG)
                continue;
            std::string filename = resourceDir + entry->d_name;
            game->levelFileList.insert(filename);
        }
    }
    closedir(levelDir);
    for(int i=0; i<game->levelFileList.size(); ++i)
    {
        debug("Loaded level %s", game->levelFileList[i].c_str());
    }

    loadCurrentLevel(game);
}

bool updateGame(GameState* game, float deltaTime)
{
    bool keepRunning = handleInput(game);
    if(game->timeTillLevelLoad > 0.0f)
    {
        game->timeTillLevelLoad -= deltaTime;
        if(game->timeTillLevelLoad <= 0.0f)
        {
            loadCurrentLevel(game);
        }
        return keepRunning;
    }

    int activeBagCount = game->bagList.size();
    int correctBagCount = 0;
    for (int i = 0; i< game->bagList.size(); ++i)
    {
        Bag* bag = game->bagList[i];
        bag->updatePosition(deltaTime);
        dge::Vector2I lastLoc = bag->lastPosition;
        MapObject* lastLocObj = game->currentLevel->map[lastLoc.y][lastLoc.x];
        if(!lastLocObj)
        {
            activeBagCount -= 1;
        }
        else if(lastLocObj->type == MapObjectType::BIN)
        {
            activeBagCount -= 1;
            Bin* bin = (Bin*)lastLocObj;
            if(bin->category == bag->category)
            {
                correctBagCount += 1;
            }
        }
    }

    if((activeBagCount+correctBagCount) < game->bagList.size())
    {
        // If the 2 counts don't add up to exactly the number of bags then we've dropped one,
        // either on the floor or in the incorrect bin, so the player loses
        game->timeTillLevelLoad = 1.0f;
    }
    else if(correctBagCount == game->bagList.size())
    {
        // If the 2 counts DO add up to the number of bags and they're all correct,
        // then the player wins!
        game->currentLevelIndex = (game->currentLevelIndex+1) % game->levelFileList.size();
        game->timeTillLevelLoad = 1.0f;
    }

    return keepRunning;
}

void cleanupGame(GameState* game)
{
}

void getWindowSettings(dge::WindowData* windowData)
{
    windowData->x = SDL_WINDOWPOS_CENTERED;
    windowData->y = SDL_WINDOWPOS_CENTERED;
    windowData->width = 640;
    windowData->height = 480;

    windowData->flags = SDL_WINDOW_OPENGL;
    windowData->title = "Lost Luggage Clone";
}
