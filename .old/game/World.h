#pragma once

#include <algorithm>
#include <sstream>
#include <vector>
#include <StateInput.h>

#include "Renderable.h"
#include "Updateable.h"

#include "Runner.h"
#include "RunnerTrail.h"
#include "Sound.h"
#include "Stats.h"
#include "TileEngine.h"
#include "Vec2.h"
#include "BackgroundRenderer.h"

static const int TILE_SIZE = 32;
static const double TOP_MIN = 300.0;

class World : public engine::Updateable, public engine::Renderable {
public:

    World(engine::Engine *engine, engine::TileEnginePtr tileEngine);

    ~World();

    void update(double timeDelta) override;

    void render(engine::Renderer *renderer) override;

    void createBlock(int left, int right, int top);

    double obstacleDistance(engine::Vec2 position);

    double floorPosition(engine::Vec2 position);

    Runner *getRunner();

    engine::Engine *getEngine();

    engine::Vec2 getCameraPosition();

    Stats &getStats();

    void displayConstants();

private:
    void drawBlocks();

    void drawBlock(const engine::Rect2 &block);

    void drawRunner();

    void drawStats(engine::Vec2 const &position);

    void addBlock(const engine::Rect2 &block);

    bool shouldGenerateNewBlock();

    void generateNewBlock();

private:
    std::unique_ptr<Runner> runner;

    engine::Vec2 position;

    engine::Engine *engine;

    engine::TileEnginePtr tileEngine;

    std::unique_ptr<engine::Texture> sawTexture;

    std::unique_ptr<engine::Texture> background;

    std::unique_ptr<engine::Texture> backgroundFar;

    std::unique_ptr<engine::Texture> backgroundMid;

    std::unique_ptr<engine::Texture> backgroundNear;

    std::vector<engine::Rect2> blocks;
    std::vector<double> blockOriginalY;
    std::vector<double> blockTimeOffset;
    std::vector<double> blockVelocity;

    std::vector<engine::SoundSamplePtr> stepSoundSamples;

    int currentStepSample = 0;

    double stepPlayTime = 0.0;

    double stepPeriod = 0.6;

    Stats stats;

    static const int PIXELS_PER_METER = 25;

    std::unique_ptr<RunnerTrail> runnerTrail;

    std::unique_ptr<engine::Entity> runnerEntity;

    std::unique_ptr<engine::StateInput> input;

    BackgroundRenderer backgroundPainter;
};

