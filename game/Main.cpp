#include <iostream>
#include <map>
#include <thread>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include "World.h"
#include "ConsoleHandler.h"

using namespace std;

static bool running = true;

int main(int argc, char **argv) {
    engine::Log log{"Main"};

    engine::Engine::Params params;
    params.screenWidth = 800;
    params.screenHeight = 600;
    params.windowTitle = "Don't Stop Running!";

    auto engine = std::unique_ptr<engine::Engine>(new engine::Engine{params});

    engine->start();

    if (!engine->isInitialized()) {
        return -1;
    }

    log.info() << "Running engine from: " << engine->getFilesystem()->getCurrentWorkingDirectory() << endl;

    auto textureFactory = engine->getTextureFactory();

    auto tiles = make_shared<engine::TileMap>();

    auto tileEngine = make_shared<engine::TileEngine>(engine->getRenderer(), tiles, textureFactory->loadImage("./data/tiles.png"));

    auto worldPtr = make_shared<World>(engine.get(), tileEngine);

    engine->registerUpdateable(worldPtr.get());
    engine->registerRenderable(worldPtr.get());

    ConsoleHandler consoleHandler{worldPtr.get()};

    while (running) {
        engine->startFrame();

        engine->update();

        engine->endFrame();
    }

    engine->stop();
    return 0;
}