#include <iostream>
#include <map>
#include <thread>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif


#include "Input.h"
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

    game->registerUpdateable(worldPtr.get());
    game->registerRenderable(worldPtr.get());

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

        game->update();

        game->endFrame();
    }

    game->stop();
    return 0;
}