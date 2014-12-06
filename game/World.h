#include <algorithm>
#include <sstream>
#include <vector>

#include "Runner.h"
#include "Sound.h"
#include "Stats.h"
#include "TileEngine.h"
#include "Vec2.h"

static const int TILE_SIZE = 32;
static const double TOP_MIN = 300.0;

class World : public engine::Game::IUpdateable, public engine::Game::IDrawable {
public:

    World(engine::GamePtr game, engine::TileEnginePtr tileEngine);

    void update(double timeDelta) override;

    void draw() override;

    void createBlock(int left, int right, int top);

    double obstacleDistance(engine::Vec2 position);

    double floorPosition(engine::Vec2 position);

    RunnerPtr getRunner();

    Stats &getStats();

    void displayConstants();

private:
    void drawBlocks();

    void drawBlock(const engine::Rect2 &block);

    void drawBackground();

    void drawRunner();

    void drawTilingBackgroundTexture(engine::Texture *texture, double offset);

    void drawStats(engine::Vec2 const &position);

    void addBlock(const engine::Rect2 &block);

    bool shouldGenerateNewBlock();

    void generateNewBlock();

    void updateTrail();

private:
    RunnerPtr runner;

    engine::Vec2 position;

    engine::GamePtr game;

    engine::TileEnginePtr tileEngine;

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

    double stepPeriod = 140;

    Stats stats;

    std::unique_ptr<engine::Texture> trailFrontBuffer;

    std::unique_ptr<engine::Texture> trailBackBuffer;

    bool trailInitialized;

    engine::Vec2 previousRunnerPosition;

    engine::Log logger;

    static const int PIXELS_PER_METER = 25;
};

