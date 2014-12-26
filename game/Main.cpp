#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <iostream>

#include <Engine.h>
#include <Log.h>
#include <TileEngine.h>

#include "ConsoleHandler.h"
#include "World.h"

using namespace std;

static bool running = true;
engine::Log _log{"Main"};

int main(int argc, char **argv) {
    engine::Engine::Params params;
    params.screenWidth = 800;
    params.screenHeight = 600;
    params.windowTitle = "Don't Stop Running!";

    auto engine = std::unique_ptr<engine::Engine>(new engine::Engine{params});

    engine->start();

    if (!engine->isInitialized()) {
        return -1;
    }

    _log.info() << "Running engine from: " << engine->getFilesystem()->getCurrentWorkingDirectory() << endl;

    auto textureFactory = engine->getTextureFactory();

    auto tiles = make_shared<engine::TileMap>();

    auto tileMap = textureFactory->loadTextureFromImage("./data/tiles.png");

    auto tileEngine = make_shared<engine::TileEngine>(engine->getRenderer(), tiles, tileMap.get());

    auto worldPtr = make_shared<World>(engine.get(), tileEngine);

    ConsoleHandler consoleHandler{worldPtr.get()};

    consoleHandler.start();

    while (running) {
        engine->startFrame();

        engine->update();

        engine->endFrame();
    }

    consoleHandler.stop();

    engine->stop();

    return 0;
}