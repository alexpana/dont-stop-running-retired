#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <Engine.h>
#include <Log.h>
#include <Rect2.h>
#include <Renderable.h>

static bool running = true;
engine::Log _log{"Main"};

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

int main() {
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