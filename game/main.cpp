#include "defines.h"

#if !TEST_PHYSICS

#include <iostream>
#include <map>
#include <thread>

#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "TileMap.h"
#include "TileEngine.h"
#include "world.h"
#include <Event.h>

using namespace std;

static bool running = true;

#define _TC(x) ((int)(x*32))

int main(int argc, char **argv) {
    engine::Game::Params params;
    params.screenWidth = 800;
    params.screenHeight = 600;
    params.windowTitle = "Don't Stop Running!";

    engine::GamePtr gamePtr = make_shared<engine::Game>();

    gamePtr->initialize(params);

    if (!gamePtr->isInitialized()) {
        return -1;
    }

    cout << "Running game from: " << gamePtr->getFilesystem().getCurrentWorkingDirectory() << endl;

    auto &imageFactory = gamePtr->getTextureFactory();

    auto tiles = make_shared<engine::TileMap>();

    auto tileEngine = make_shared<engine::TileEngine>(gamePtr->getRenderer(), tiles, imageFactory.loadImage("./data/tiles.png"));

    auto worldPtr = make_shared<World>(gamePtr, tileEngine);

    gamePtr->registerUpdateable(worldPtr);
    gamePtr->registerDrawable(worldPtr);

    worldPtr->createBlock(_TC(0), _TC(8), _TC(20));

    auto runner = worldPtr->getRunner();

    // create input thread
    std::thread{[&worldPtr, &runner]() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (true) {
            std::string cmd;
            int n;
            std::cin >> cmd;

            if (cmd == "setv") {
                std::cin >> n;
                runner->setSpeed(n);
            }

            if (cmd == "setg") {
                std::cin >> n;
                runner->setGravity(n);
            }

            if (cmd == "setj") {
                std::cin >> n;
                runner->setJumpVelocity(n);
            }

            if (cmd == "p") {
                worldPtr->displayConstants();
            }
        }
#pragma clang diagnostic pop
    }}.detach();

    while (running) {
        gamePtr->startFrame();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            using engine::Event;

            if (Event::isQuit(e)) {
                running = false;
            }

            if (Event::isKeyDown(e, SDLK_SPACE)) {
                runner->startJump();
                runner->startJump();
            }

            if (Event::isKeyDown(e, SDLK_SPACE)) {
                runner->startJump();

            }

            if (Event::isKeyUp(e, SDLK_SPACE)) {
                runner->endJump();
            }

            if (Event::isKeyDown(e, SDLK_1)) {
                runner->increaseSpeed();
                worldPtr->displayConstants();
            }

            if (Event::isKeyDown(e, SDLK_2)) {
                runner->decreaseSpeed();
                worldPtr->displayConstants();
            }

            if (Event::isKeyDown(e, SDLK_3)) {
                runner->increaseJumpVelocity();
                worldPtr->displayConstants();
            }

            if (Event::isKeyDown(e, SDLK_4)) {
                runner->decreaseJumpVelocity();
                worldPtr->displayConstants();
            }

            if (Event::isKeyDown(e, SDLK_5)) {
                runner->increaseGravity();
                worldPtr->displayConstants();
            }

            if (Event::isKeyDown(e, SDLK_6)) {
                runner->decreaseGravity();
                worldPtr->displayConstants();
            }
        }

        gamePtr->update();

        gamePtr->endFrame();
    }

    return 0;
}

#endif