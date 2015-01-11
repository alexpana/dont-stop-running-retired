#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <iostream>

#include <Engine.h>
#include <Log.h>
#include <Renderable.h>
#include <Sprite.h>
#include <SpriteSheet.h>
#include <SpriteSheetLoader.h>
#include <TileEngine.h>
#include <Version.h>

#include "ConsoleHandler.h"
#include "World.h"

using namespace std;

static bool running = true;
engine::Log _log{"Main"};

int runGame() {
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

    auto textureFactory = engine->getTextureLoader();

    auto tiles = make_unique<engine::TileMap>();

    auto tileMap = textureFactory->load("./data/tiles.png");

    auto tileEngine = make_shared<engine::TileEngine>(engine->getRenderer(), tiles.get(), tileMap.get());

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

class AssetRenderer : public engine::Renderable {
public:
    AssetRenderer(engine::Engine *engine, std::unique_ptr<engine::Texture> &&texture) :
            engine(engine),
            texture(std::move(texture)) {
    };

    void render(engine::Renderer *renderer) override {
        using engine::Vec2;

        renderer->setTextureOrigin(texture->getBounds().centerOfMass());

        renderer->drawTexture(texture.get(), Vec2{000, 000}, engine->getElapsedTime() * 100);

        renderer->setTextureOrigin({0, 0});
    }

private:
    engine::Engine *engine;
    std::unique_ptr<engine::Texture> texture;
};

int runAssetView() {
    engine::Engine::Params params;
    params.screenWidth = 400;
    params.screenHeight = 400;
    params.windowTitle = "Asset Viewer";

    auto engine = std::make_unique<engine::Engine>(params);

    engine->start();

    auto texture = engine->getTextureLoader()->load("data/tree_test_0.png");

    auto textureRenderer = std::make_unique<AssetRenderer>(engine.get(), std::move(texture));

    engine->registerRenderable(textureRenderer.get());

    if (!engine->isInitialized()) {
        return -1;
    }

    while (running) {
        engine->startFrame();

        engine->update();

        engine->endFrame();
    }

    engine->stop();

    return 0;
}

int main(int argc, char **argv) {
    engine::Version version;
    engine::getVersion(&version);
    _log.info() << "Running engine version " << version << std::endl;
    return runGame();
}