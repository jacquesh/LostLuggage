#include <GL/gl3w.h>

#include "imgui.h"

#include "game.h"
#include "dge_debug.h"
#include "dge_math.h"
#include "dge_render.h"

#include "render.h"

GLuint cursorTexture;

GLuint pixelTexture;
GLuint towerTexture;
GLuint enemyTexture;

// TODO: Dump everything in a texture atlas
void loadRenderData()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    dge_loadDefaultShaders();
    dge_loadTextRenderer();
    dge_allocateAndLoadTexture("resources/cursor.png", &cursorTexture);

    dge_allocateAndLoadTexture("resources/tower.png", &towerTexture);
    dge_allocateAndLoadTexture("resources/pixel.png", &pixelTexture);
    dge_allocateAndLoadTexture("resources/enemy.png", &enemyTexture);
}

static Vector4 categoryColours[9] = {Vector4(0.0f, 0.0f, 1.0f, 1.0f),
                                     Vector4(0.0f, 1.0f, 0.0f, 1.0f),
                                     Vector4(0.0f, 1.0f, 1.0f, 1.0f),
                                     Vector4(1.0f, 0.0f, 0.0f, 1.0f),
                                     Vector4(1.0f, 0.0f, 1.0f, 1.0f),
                                     Vector4(1.0f, 1.0f, 0.0f, 1.0f),
                                     Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                                     Vector4(0.0f, 0.0f, 0.0f, 1.0f),
                                     Vector4(0.5f, 0.5f, 0.5f, 1.0f),
                                    };

void renderGame(GameState* game)
{
    glClear(GL_COLOR_BUFFER_BIT);

    Vector2 cellSize(grid_size, grid_size);
    Vector4 white(1.0f, 1.0f, 1.0f, 1.0f);
    for(int y=0; y<game->currentLevel->height; ++y)
    {
        for(int x=0; x<game->currentLevel->width; ++x)
        {
            MapObject* mapObj = game->currentLevel->map[y][x];
            if(!mapObj)
                continue;

            Vector2 cellCentre(x*grid_size, y*grid_size);
            switch(mapObj->type)
            {
                case MapObjectType::conveyer:
                {
                    Conveyer* conv = (Conveyer*)mapObj;
                    dge_renderQuad(game->camera, cellCentre, cellSize, 0.0f, white);
                    const char* dirChar = "";
                    switch(conv->dir)
                    {
                        case Direction::left: dirChar = "<"; break;
                        case Direction::right: dirChar = ">"; break;
                        case Direction::up: dirChar = "^"; break;
                        case Direction::down: dirChar = "v"; break;
                        default: dirChar = "x";
                    }
                    dge_renderString(game->camera, dirChar, 1, cellCentre, grid_size/2.0f, white);
                } break;

                case MapObjectType::bin:
                {
                    Bin* bin = (Bin*)mapObj;
                    dge_renderQuad(game->camera, cellCentre, cellSize, 0.0f, white);
                    dge_renderQuad(game->camera, cellCentre, cellSize*0.8f, 0.0f, categoryColours[bin->category]);
                } break;
            }
        }
    }

    Vector4 red(1.0f, 0.0f, 0.0f, 1.0f);
    for(int bagIndex=0; bagIndex<game->bagList.size(); ++bagIndex)
    {
        Bag bag = *game->bagList[bagIndex];
        Vector2 position = bag.position * grid_size;
        dge_renderQuad(game->camera, position, bag.size, 0.0f, red);
        dge_renderQuad(game->camera, position, bag.size, 0.0f, categoryColours[bag.category]);
    }

/*    ImVec2 windowLoc(50.0f, 50.0f);
    ImVec2 windowSize(200.0f, 200.f);
    int UIFlags = ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("MainMenu", 0, UIFlags);
    ImGui::SetWindowPos(windowLoc);
    ImGui::SetWindowSize(windowSize);

    ImGui::Text("Lost Luggage Clone");
    if(ImGui::Button("Start", ImVec2(100, 50)))
    {
    }

    ImGui::End();
    */
}

void lateRender(GameState* game)
{
}
