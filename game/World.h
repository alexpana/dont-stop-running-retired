#include <vector>
#include <assert.h>
#include <algorithm>

#include "Rect2.h"
#include "Runner.h"
#include "Stats.h"
#include "TileEngine.h"
#include "Vec2.h"

static const int TILE_SIZE = 32;
static const double TOP_MIN = 300.0;

class World : public engine::Game::IUpdateable, public engine::Game::IDrawable {
public:

    World(engine::GamePtr game, engine::TileEnginePtr tileEngine) :
            tileEngine(tileEngine),
            game(game) {
        runner = std::make_shared<Runner>(this);
        game->registerUpdateable(runner);

        background = game->getTextureFactory().loadImage("data/bg.png");
    }

    void update(double timeDelta) override {
        position.x = -runner->getPosition().x + 60;
        stats.milesRan += 60;

        if (shouldGenerateNewBlock()) {
            generateNewBlock();
        }

        // move blocks
        for (int i = 0; i < blocks.size(); ++i) {
            long double delta = std::sin(blockTimeOffset[i]) * 0.3;
            blocks[i].y += delta;
            blockTimeOffset[i] += blockVelocity[i];
        }
    }

    void draw() override {
        using namespace engine;

        game->getRenderer()->drawTexture(background, engine::Vec2((int) position.x % background->getWidth(), 0));
        game->getRenderer()->drawTexture(background, engine::Vec2((int) position.x % background->getWidth() + background->getWidth(), 0));

        // draw blocks
        for (const auto &block : blocks) {
            drawBlock(block);
        }

        auto runnerPosition = runner->getPosition();
        auto runnerSize = runner->getSize();

        // draw runner
        game->getRenderer()->setColor(0x601030FF);
        game->getRenderer()->fillRect(engine::Rect2(60.0, runnerPosition.y, runnerSize.x, runnerSize.y));
    }

    void drawBlock(const engine::Rect2 &block);

    void createBlock(int left, int right, int top) {
        addBlock(engine::Rect2(left, right, top, 800));
    }

    double obstacleDistance(engine::Vec2 position) {
        double minDistance = 1000;

        for (auto &block : blocks) {
            if (block.topLeft().y < position.y && block.topLeft().x > position.x) {
                minDistance = std::min(minDistance, block.topLeft().x - position.x);
            }
        }

        return minDistance;
    }

    double floorPosition(engine::Vec2 position) {
        for (auto &block : blocks) {
            if (block.topLeft().x < position.x && block.topRight().x > position.x) {
                return block.topLeft().y;
            }
        }

        return 1000;
    }

    RunnerPtr getRunner() {
        return runner;
    }

    void displayConstants() {
        std::cout << "Gravity:             " << runner->gravity << std::endl;
        std::cout << "Jump start velocity: " << runner->longJumpStartVelocity << std::endl;
        std::cout << "Speed:               " << runner->velocity.x << std::endl;
    }

    Stats &getStats() {
        return stats;
    }

private:

    void addBlock(const engine::Rect2 &block) {
        blocks.push_back(block);

        blockOriginalY.push_back(block.y);

        blockTimeOffset.push_back(game->getRandom().nextInt(100) / 10.0);

        blockVelocity.push_back(0.005 + game->getRandom().nextInt(20) / 20000.0);
    }

    bool shouldGenerateNewBlock() {
        static const int offset = 50;

        engine::Rect2 lastBlock = blocks[blocks.size() - 1];
        return lastBlock.x + lastBlock.w + position.x < game->getScreenWidth() + 50;
    }

    void generateNewBlock() {
        using namespace engine;

        Random &random = game->getRandom();
        Rect2 lastBlock = blocks[blocks.size() - 1];
        double lastBlockOriginalHeight = blockOriginalY[blocks.size() - 1];

        double left = lastBlock.x + lastBlock.w;

        int r1 = random.nextInt(0, 100);
        if (r1 > 30) {
            left += random.nextInt(100, 150);
        }

        double top = std::max(TOP_MIN, lastBlockOriginalHeight + random.nextInt(6) * 20 - 60);
        double width = random.nextInt(6, 12) * TILE_SIZE;

        addBlock(engine::Rect2{left, top, width, 100.0});
    }

private:
    RunnerPtr runner;

    engine::Vec2 position;

    engine::GamePtr game;

    engine::TileEnginePtr tileEngine;

    engine::TexturePtr background;

    std::vector<engine::Rect2> blocks;
    std::vector<double> blockOriginalY;
    std::vector<double> blockTimeOffset;
    std::vector<double> blockVelocity;

    Stats stats;
};

inline void World::drawBlock(engine::Rect2 const &block) {
    // Skip invisible blocks
    if (block.x + block.w + position.x < 0) return;

    double screenX = block.x + position.x;

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        tileEngine->drawTile(screenX + i * TILE_SIZE, block.y, engine::GRASS_TOP);
    }

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j * TILE_SIZE + block.y > game->getScreenHeight()) break;
            tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE, engine::GROUND);
        }
    }
}