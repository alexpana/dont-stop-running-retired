#include <iostream>
#include <map>
#include <thread>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>

#include "Event.h"
#include "Sound.h"
#include "TileMap.h"
#include "TileEngine.h"
#include "World.h"

using namespace std;

static bool running = true;

int main(int argc, char **argv) {
    engine::Game::Params params;
    params.screenWidth = 800;
    params.screenHeight = 600;
    params.windowTitle = "Don't Stop Running!";

    engine::GamePtr game = make_shared<engine::Game>(params);

    engine::Log log{"Main"};

    game->start();

    if (!game->isInitialized()) {
        return -1;
    }

    log.info() << "Running game from: " << game->getFilesystem()->getCurrentWorkingDirectory() << endl;

    auto textureFactory = game->getTextureFactory();

    auto tiles = make_shared<engine::TileMap>();

    auto tileEngine = make_shared<engine::TileEngine>(game->getRenderer(), tiles, textureFactory->loadImage("./data/tiles.png"));

    auto worldPtr = make_shared<World>(game, tileEngine);

    game->registerUpdateable(worldPtr);
    game->registerDrawable(worldPtr);

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
        game->startFrame();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            using engine::Event;

            if (Event::isQuit(e)) {
                running = false;
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

            if (Event::isKeyDown(e, SDLK_e)) {
                game->getSound()->playSampleOnce(worldPtr->getFootstep());
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(nullptr);

        if (state[SDL_SCANCODE_SPACE]) {
            runner->addJumpForce();
        }

        game->update();

        game->endFrame();
    }

    game->stop();
    return 0;
}