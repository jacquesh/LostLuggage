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

GLuint roadTex_deadend;
GLuint roadTex_straight;
GLuint roadTex_turn;
GLuint roadTex_tjunction;
GLuint roadTex_crossroads;

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

    dge_allocateAndLoadTexture("resources/road_deadend.png", &roadTex_deadend);
    dge_allocateAndLoadTexture("resources/road_vertical.png", &roadTex_straight);
    dge_allocateAndLoadTexture("resources/road_corner.png", &roadTex_turn);
    dge_allocateAndLoadTexture("resources/road_tjunction.png", &roadTex_tjunction);
    dge_allocateAndLoadTexture("resources/road_crossroads.png", &roadTex_crossroads);
}

void renderGame(GameState* game)
{
    glClear(GL_COLOR_BUFFER_BIT);

    ImVec2 windowLoc(40.0f, 300.0f);
    ImVec2 windowSize(400.0f, 200.f);
    int UIFlags = ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("MainMenu", 0, UIFlags);
    ImGui::SetWindowPos(windowLoc);
    ImGui::SetWindowSize(windowSize);

    ImGui::Text("Treasure Defender Clone!");
    if(ImGui::Button("Start", ImVec2(100, 50)))
    {
        game->currentScene = SCENE_INGAME;
    }
    if(ImGui::Button("Quit", ImVec2(100, 50)))
    {
        // TODO: Quit the game
    }

    ImGui::End();
}

void lateRender(GameState* game);
