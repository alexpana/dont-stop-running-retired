#include "defines.h"

#if TEST_PHYSICS

#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include <windows.h>


#include "game.h"
#include "image_factory.h"
#include "graphics/renderer.h"
#include "tile_map.h"
#include "tile_engine.h"
#include "world.h"
#include "timer.h"
#include "filesystem.h"
#include "physics/collision_system.h"

using namespace std;

static bool running = true;

#define _TC(x) ((int)(x*32))


void draw(SDL_Renderer* renderer, const engine::Rect2& rect) {
    SDL_RenderDrawRect(renderer, &rect.toSDLRect());
}

void draw(SDL_Renderer* renderer, const engine::Vec2& from, const engine::Vec2& to) {
    SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
}

void draw(SDL_Renderer* renderer, const engine::Vec2& point) {
    SDL_Rect rect = {point.x - 2, point.y - 2, 4, 4};
    SDL_RenderDrawRect(renderer, &rect);
}

int foo(int a) {
    return 42;
}

void bar() {
    int b;
    return;
};

int main(int argc, char** argv)
{
    using namespace engine;

    Game::Params params = {800, 600, "Don't Stop Running!"};

    auto game = Game();

    cout << "Running game from: " << game.getFilesystem().getCurrentWorkingDirectory() << endl;

    bool initialized = game.initialize(params);

    if (!initialized)
    {
        printf("Could not initialize the game.");
        return -1;
    }

    engine::graphics::Renderer renderer = game.getRenderer();
    physics::CollisionDetector collisionDetector;

    Rect2 rect = Rect2(30, 40, 200, 200);

    bool isRayActive = false;
    Vec2 rayOrigin;
    Vec2 rayDirection;

    while (game.isRunning())
    {
        game.startFrame();

        SDL_Event e;

        //Handle events on queue 
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type) {
            case SDL_QUIT:
                game.stopRunning();
                break;

            case SDL_MOUSEBUTTONDOWN:
                isRayActive = true;
                rayOrigin = Vec2(e.button.x, e.button.y);
                break;

            case SDL_MOUSEMOTION:
                rayDirection = Vec2(e.motion.x, e.motion.y);
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    // Start jump
                }
                break;

            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    // End jump
                }
                break;
            }
        }

        renderer.setColor(32, 33, 34);
        renderer.clear();

        renderer.setColor(200, 20, 20);
        renderer.drawRect(rect);

        renderer.setColor(20, 200, 20);
        renderer.drawLine(rayOrigin, rayDirection);

        physics::CollisionDetector::Result result = collisionDetector.rayIntersection(rayOrigin, rayDirection - rayOrigin, rect);

        if (result.hasIntersection) {
            renderer.setColor(20, 20, 200);
            renderer.drawCircle(result.intersection, 2);
        }

        game.endFrame();
    }

    return 0;
}

#endif