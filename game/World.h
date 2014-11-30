#include <vector>
#include <assert.h>
#include <algorithm>

#include "Rect2.h"
#include "Runner.h"
#include "TileEngine.h"
#include "Vec2.h"

static const int TILE_SIZE = 32;
static const double TOP_MIN = 300.0;

class World : public engine::Game::IUpdateable, public engine::Game::IDrawable {
public:

    World(engine::GamePtr game, engine::TileEnginePtr tileEngine) :
            m_tileEngine(tileEngine),
            m_game(game) {
        m_runner = std::make_shared<Runner>(this);
        m_game->registerUpdateable(m_runner);

        m_background = m_game->getTextureFactory().loadImage("data/bg.png");
    }

    void update(double timeDelta) override {
        m_position.x = -m_runner->getPosition().x + 60;

        if (shouldGenerateNewBlock()) {
            generateNewBlock();
        }
    }

    void draw() override {
        m_game->getRenderer()->drawTexture(m_background, engine::Vec2((int) m_position.x % m_background->getWidth(), 0));
        m_game->getRenderer()->drawTexture(m_background, engine::Vec2((int) m_position.x % m_background->getWidth() + m_background->getWidth(), 0));

        // draw blocks
        for (auto block : m_blocks) {
            drawBlock(block);
        }

        auto runnerPosition = m_runner->getPosition();
        auto runnerSize = m_runner->getSize();

        // draw runner
        m_game->getRenderer()->setColor(0x601030FF);
        m_game->getRenderer()->fillRect(engine::Rect2(
                60.0,
                runnerPosition.y,
                runnerSize.x,
                runnerSize.y));
    }

    void drawBlock(const engine::Rect2 &block);

    void createBlock(int left, int right, int top) {
        addBlock(engine::Rect2(left, right, top, 800));
    }

    double obstacleDistance(engine::Vec2 position) {
        double minDistance = 1000;

        for (auto &block : m_blocks) {
            if (block.topLeft().y < position.y && block.topLeft().x > position.x) {
                minDistance = std::min(minDistance, block.topLeft().x - position.x);
            }
        }

        return minDistance;
    }

    double floorPosition(engine::Vec2 position) {
        for (auto &block : m_blocks) {
            if (block.topLeft().x < position.x && block.topRight().x > position.x) {
                return block.topLeft().y;
            }
        }

        return 1000;
    }

    RunnerPtr getRunner() {
        return m_runner;
    }

    void displayConstants() {
        std::cout << "Gravity:             " << m_runner->gravity << std::endl;
        std::cout << "Jump start velocity: " << m_runner->longJumpStartVelocity << std::endl;
        std::cout << "Speed:               " << m_runner->velocity.x << std::endl;
    }

private:

    void addBlock(const engine::Rect2 &block) {
        m_blocks.push_back(block);
    }

    bool shouldGenerateNewBlock() {
        static const int offset = 50;

        engine::Rect2 lastBlock = m_blocks[m_blocks.size() - 1];
        return lastBlock.x + lastBlock.w + m_position.x < m_game->getScreenWidth() + 50;
    }

    void generateNewBlock() {
        engine::Random &random = m_game->getRandom();
        engine::Rect2 lastBlock = m_blocks[m_blocks.size() - 1];

        double left = lastBlock.x + lastBlock.w;

        int r1 = random.nextInt(0, 100);
        if (r1 > 30) {
            r1 = random.nextInt(100, 150);

            left += random.nextInt(100, 150);
        }

        double top = std::max(TOP_MIN, lastBlock.y + random.nextInt(6) * 20 - 60);
        double width = random.nextInt(6, 12) * TILE_SIZE;

        addBlock(engine::Rect2{left, top, width, 800.0});
    }


private:
    RunnerPtr m_runner;

    engine::Vec2 m_position;

    engine::GamePtr m_game;

    engine::TileEnginePtr m_tileEngine;

    engine::TexturePtr m_background;

    std::vector<engine::Rect2> m_blocks;
};

inline void World::drawBlock(engine::Rect2 const &block) {
    // Skip invisible blocks
    if (block.x + block.w + m_position.x < 0) return;

    double screenX = block.x + m_position.x;

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        m_tileEngine->drawTile(screenX + i * TILE_SIZE, block.y, engine::GRASS_TOP);
    }

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j * TILE_SIZE + block.y > m_game->getScreenHeight()) break;
            m_tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE, engine::GROUND);
        }
    }
}