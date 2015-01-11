#pragma once

#include "TestingFramework.h"

#include <assert.h>
#include <Engine.h>
#include <SpriteSheet.h>
#include <SpriteSheetLoader.h>
#include <Sprite.h>

using namespace engine;

class TestSpriteSheetLoader : public test::Test {

    BEGIN_TEST(simpleLoader)
        engine::Engine::Params params;
        params.screenWidth = 400;
        params.screenHeight = 400;
        params.windowTitle = "Asset Viewer";

        auto engine = std::make_unique<engine::Engine>(params);

        engine->start();

        engine::SpriteSheetLoader loader{engine->getTextureLoader()};
        std::unique_ptr<engine::SpriteSheet> spriteSheet = loader.load("data/spritesheet.json");

        engine::Sprite *wall1 = spriteSheet->getSprite("wall_1");
        assert(wall1 != nullptr);

        engine::Sprite *wall2 = spriteSheet->getSprite("wall_2");
        assert(wall2 != nullptr);

        engine->stop();
    END_TEST

    void registerTests() override {
        REGISTER_TEST(simpleLoader);
    }
};