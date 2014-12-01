#include <algorithm>
#include <sstream>
#include <vector>

#include "Runner.h"
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

    void drawStats(engine::Vec2 const &position);

    void addBlock(const engine::Rect2 &block);

    bool shouldGenerateNewBlock();

    void generateNewBlock();

private:
    RunnerPtr runner;

    engine::Vec2 position;

    engine::GamePtr game;

    engine::TileEnginePtr tileEngine;

    engine::TexturePtr background;

    engine::TexturePtr backgroundFar;

    engine::TexturePtr backgroundMid;

    engine::TexturePtr backgroundNear;

    std::vector<engine::Rect2> blocks;
    std::vector<double> blockOriginalY;
    std::vector<double> blockTimeOffset;
    std::vector<double> blockVelocity;

    Stats stats;

    static const int PIXELS_PER_METER = 25;
};

