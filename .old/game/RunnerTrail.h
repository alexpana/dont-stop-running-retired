#pragma once

#include "Updateable.h"
#include "Engine.h"
#include <Vec2.h>

class World;

class RunnerTrail : engine::Updateable {
public:
    RunnerTrail(World *world);

    void update(double deltaTime) override;

    engine::Texture *getTrailTexture();

private:
    World *world;

    bool trailInitialized = false;

    engine::Vec2 previousRunnerPosition;

    std::unique_ptr<engine::Texture> frontBuffer;

    std::unique_ptr<engine::Texture> backBuffer;

    std::unique_ptr<engine::Texture> trailGradient;

    void drawTrailSegment(int interpolatedX, int interpolatedY);

    int totalOffset = 0;
};