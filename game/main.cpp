#include "defines.h"

#if !TEST_PHYSICS

#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "TileMap.h"
#include "TileEngine.h"
#include "Timer.h"
#include "world.h"

using namespace std;

static bool running = true;

#define _TC(x) ((int)(x*32))

class FrameCounter {
public:
    FrameCounter(const engine::GamePtr game) : m_game(game), lastFrameCount(m_game->getFrameCount()) {}

    int getMilli() {
        return m_timer.getMilli();
    }

    void restart() {
        m_timer.restart();
        lastFrameCount = m_game->getFrameCount();
    }

    double getFPS() {
        return static_cast<double>(m_game->getFrameCount() - lastFrameCount) * 1000 / m_timer.getMilli();
    }

private:
    const engine::GamePtr m_game;

    engine::Timer m_timer;

    int lastFrameCount = 0;
};

int main(int argc, char** argv)
{
    engine::Game::Params params;
    params.screenWidth = 800;
    params.screenHeight = 600;
    params.windowTitle = "Don't Stop Running!";

    engine::GamePtr gamePtr = make_shared<engine::Game>();
    auto& game = *(gamePtr.get());

    cout << "Running game from: " << game.getFilesystem().getCurrentWorkingDirectory() << endl;

    bool initialized = game.initialize(params);

    if (!initialized) 
    {
        return -1;
    }

    auto& imageFactory = game.getTextureFactory();

    auto tiles = make_shared<engine::TileMap>();

    auto tileEngine = make_shared<engine::TileEngine>(game.getRenderer(), tiles, imageFactory.loadImage("./data/tiles.png"));

    auto world = World{gamePtr, tileEngine};

    world.createBlock(_TC(0), _TC(8), _TC(4));
    world.createBlock(_TC(5), _TC(12.5), _TC(4));
    world.createBlock(_TC(12), _TC(13), _TC(2));
    world.createBlock(_TC(16), _TC(12), _TC(6));

    auto frameCounter = FrameCounter{gamePtr};
    auto timer = engine::Timer{};

    if (!game.isInitialized())
    {
        printf("Could not initialize the game.");
        return -1;
    }

    while (running)
    {
        SDL_Event e;

        game.getRenderer()->setColor(0x303132ff);
        game.getRenderer()->clear();

        //Handle events on queue 
        while(SDL_PollEvent(&e) != 0 ) 
        { 
            //User requests quit 
            if(e.type == SDL_QUIT)
            {
                running = false;
            } 

            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    // Start jump
                }
            }

            if (e.type == SDL_KEYUP)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    // End jump
                }
            }

        }

        if (frameCounter.getMilli() > 2000) {
            double fps = frameCounter.getFPS();

            cout << "FPS: " << fps << std::endl;

            frameCounter.restart();
        }

        int milli = timer.getMilli();
        timer.restart();

        world.update(milli);

        world.draw();

        game.endFrame();
    }

    return 0;
}

#endif