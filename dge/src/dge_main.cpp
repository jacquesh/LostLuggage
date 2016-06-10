#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_version.h>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "dge.h"
#include "dge_render.h"
#include "dge_debug.h"

#include "game.h"

// =============
// The main DGE API, all you need is to link with a file that implements these functions
// =============
void getWindowSettings(dge::WindowData* windowData);

void initGame(GameState* game);
bool updateGame(GameState* game, float deltaTime);
void renderGame(GameState* game);
void lateRender(GameState* game);
void cleanupGame(GameState* game);

int main()
{
    // TODO: Theres a (potential) difference between the version of SDL that we compiled with
    //       and the version we're running with (so maybe thats worth mentioning here)
    debug("Initializing SDL version %d.%d.%d",
            SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Initialization Error",
                                 "Unable to initialize SDL", 0);
        debug("Error when trying to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    dge::WindowData windowData = {};
    getWindowSettings(&windowData);
    SDL_Window* window = SDL_CreateWindow(windowData.title,
                                         windowData.x, windowData.y,
                                         windowData.width, windowData.height,
                                         windowData.flags);
    if(!window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Initialization Error",
                                 "Unable to create window!", 0);
        debug("Error when trying to create SDL Window: %s", SDL_GetError());

        SDL_Quit();
        return 1;
    }
    if(!dge::initRenderer(window))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Initialization Error",
                                 "Unable to create renderer!", 0);
        debug("Error when trying to create Renderer: %s", SDL_GetError());

        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);
    ImGui_ImplSdlGL3_Init(window);

    // TODO: SDL_ShowCursor(0) doesn't appear to work, so we have this workaround for now
    const uint8 zero = 0;
    SDL_Cursor* cursor = SDL_CreateCursor(&zero, &zero, 1,1, 0,0);
//    SDL_SetCursor(cursor);

    int maxControllerIndex = SDL_NumJoysticks();
    debug("We can have at most %d controllers", maxControllerIndex);
    for(int index=0; index<maxControllerIndex; ++index)
    {
        if(SDL_IsGameController(index))
        {
            SDL_GameController* controller = SDL_GameControllerOpen(index);
            if(controller)
            {
                debug("Successfully opened controller %d with name \"%s\"",
                        index, SDL_GameControllerName(controller));
                break;
            }
            else
            {
                debug("Unable to open game controller %d: %s", index, SDL_GetError());
            }
        }
    }

    uint64 performanceFreq = SDL_GetPerformanceFrequency();
    uint64 performanceCounter = SDL_GetPerformanceCounter();

    uint64 tickRate = 64;
    uint64 tickDuration = performanceFreq/tickRate;
    uint64 nextTickTime = performanceCounter;
    float frameTime = 1.0f/(float)tickRate;

    int maxTicksWithoutDraw = 5;

    uint64 secondBoundary = performanceCounter + performanceFreq;
    uint64 updateCount = 0;
    uint64 renderCount = 0;

    GameState game = {};
    initGame(&game);

    bool running = true;

    glPrintError(true);
    debug("Setup complete, start running game...");
    while(running)
    {
        uint64 newPerfCount = SDL_GetPerformanceCounter();
        uint64 deltaPerfCount = newPerfCount - performanceCounter;
        performanceCounter = newPerfCount;

        float deltaTime = ((float)deltaPerfCount)/((float)performanceFreq);
#if 0
        // Debug: Print the number of updates and renders that happen every second
        //debug("Frame time: %f - %f FPS", deltaTime, 1.0f/deltaTime);
        if(newPerfCount > secondBoundary)
        {
            debug("Updates: %d", updateCount);
            debug("Renders: %d", renderCount);
            updateCount = 0;
            renderCount = 0;
            secondBoundary += performanceFreq;
        }
#endif
        // Game Tick
        int ticksWithoutDraw = 0;
        while((SDL_GetPerformanceCounter() > nextTickTime) &&
              (ticksWithoutDraw < maxTicksWithoutDraw))
        {
            // TODO: This deltaTime passed into update here is probably incorrect, surely
            //       if this loop runs multiple times, it'll progress the game by more time
            //       than has actually passed?
            running &= updateGame(&game, frameTime);

            nextTickTime += tickDuration;
            ++ticksWithoutDraw;
            ++updateCount;
        }

        // Rendering
        ++renderCount;
        ImGui_ImplSdlGL3_NewFrame(window);
        renderGame(&game);
        ImGui::Render();

        lateRender(&game);
        SDL_GL_SwapWindow(window);
        glPrintError(false);

        // Sleep till next scheduled update
        newPerfCount = SDL_GetPerformanceCounter();
        if(newPerfCount < nextTickTime)
        {
            deltaPerfCount = nextTickTime - newPerfCount;
            float sleepSeconds = (float)deltaPerfCount/(float)performanceFreq;
            uint32 sleepMS = (uint32)(sleepSeconds*1000);
            SDL_Delay(sleepMS);
        }
    }

    cleanupGame(&game);
    ImGui_ImplSdlGL3_Shutdown();
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
