#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <Memory.h>

#include "TestingFramework.h"

#include "TestBones.h"
#include "TestMath.h"
#include "TestSpriteSheetLoader.h"
#include "TestIdentities.h"

int main() {
    test::TestRunner runner{};

    runner.registerTestClass(std::make_unique<TestBones>(), "TestBones");
    runner.registerTestClass(std::make_unique<TestMath>(), "TestMath");
    runner.registerTestClass(std::make_unique<TestSpriteSheetLoader>(), "TestSpriteSheetLoader");
    runner.registerTestClass(std::make_unique<TestIdentities>(), "TestIdentities");

    runner.run();

    return 0;
}
