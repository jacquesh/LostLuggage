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
    dge::loadDefaultShaders();
    dge::loadTextRenderer();
    dge::allocateAndLoadTexture("resources/gfx/cursor.png", &cursorTexture);

    dge::allocateAndLoadTexture("resources/gfx/tower.png", &towerTexture);
    dge::allocateAndLoadTexture("resources/gfx/pixel.png", &pixelTexture);
    dge::allocateAndLoadTexture("resources/gfx/enemy.png", &enemyTexture);
}

static dge::Vector4 categoryColours[9] = {dge::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
                                          dge::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
                                          dge::Vector4(0.0f, 1.0f, 1.0f, 1.0f),
                                          dge::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
                                          dge::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
                                          dge::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
                                          dge::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                                          dge::Vector4(0.0f, 0.0f, 0.0f, 1.0f),
                                          dge::Vector4(0.5f, 0.5f, 0.5f, 1.0f),
                                         };

void renderGame(GameState* game)
{
    glClear(GL_COLOR_BUFFER_BIT);

    dge::Vector2 cellSize(grid_size, grid_size);
    dge::Vector4 white(1.0f, 1.0f, 1.0f, 1.0f);
    for(int y=0; y<game->currentLevel->height; ++y)
    {
        for(int x=0; x<game->currentLevel->width; ++x)
        {
            MapObject* mapObj = game->currentLevel->map[y][x];
            if(!mapObj)
                continue;

            dge::Vector2 cellCentre(x*grid_size, y*grid_size);
            switch(mapObj->type)
            {
                case MapObjectType::CONVEYER:
                {
                    Conveyer* conv = (Conveyer*)mapObj;
                    dge::renderQuad(game->camera, cellCentre, cellSize, 0.0f, white);
                    const char* dirChar = "";
                    switch(conv->dir)
                    {
                        case Direction::LEFT: dirChar = "<"; break;
                        case Direction::RIGHT: dirChar = ">"; break;
                        case Direction::UP: dirChar = "^"; break;
                        case Direction::DOWN: dirChar = "v"; break;
                        default: dirChar = "x";
                    }
                    dge::renderString(game->camera, dirChar, 1, cellCentre, grid_size/2.0f, white);
                } break;

                case MapObjectType::BIN:
                {
                    Bin* bin = (Bin*)mapObj;
                    dge::renderQuad(game->camera, cellCentre, cellSize, 0.0f, white);
                    dge::renderQuad(game->camera, cellCentre, cellSize*0.8f, 0.0f, categoryColours[bin->category]);
                } break;

                case MapObjectType::WALL:
                {
                    Wall* wall = (Wall*)mapObj;
                    switch (wall->dir)
                    {
                      case Direction::UP    : dge::renderQuad(game->camera, cellCentre+dge::Vector2(0,0.3*grid_size), cellSize*dge::Vector2(0.6f,0.2f), 0.0f, dge::Vector4 (0.5f,0.f,0.f,1.f)); break;
                      case Direction::DOWN  : dge::renderQuad(game->camera, cellCentre+dge::Vector2(0,-0.3*grid_size), cellSize*dge::Vector2(0.6f,0.2f), 0.0f, dge::Vector4 (0.5f,0.f,0.f,1.f)); break;
                      case Direction::LEFT  : dge::renderQuad(game->camera, cellCentre+dge::Vector2(-0.3*grid_size,0), cellSize*dge::Vector2(0.2f,0.6f), 0.0f, dge::Vector4 (0.5f,0.f,0.f,1.f)); break;
                      case Direction::RIGHT : dge::renderQuad(game->camera, cellCentre+dge::Vector2(0.3*grid_size,0), cellSize*dge::Vector2(0.2f,0.6f), 0.0f, dge::Vector4 (0.5f,0.f,0.f,1.f)); break;
                      default:;
                    }
                } break;
            }
        }
    }

    dge::Vector4 red(1.0f, 0.0f, 0.0f, 1.0f);
    for(int bagIndex=0; bagIndex<game->bagList.size(); ++bagIndex)
    {
        Bag bag = *game->bagList[bagIndex];
        dge::Vector2 position = bag.position * grid_size;
        dge::Vector2 size = bag.size * grid_size;
        dge::renderQuad(game->camera, position, size, 0.0f, red);
        dge::renderQuad(game->camera, position, size, 0.0f, categoryColours[bag.category]);
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
