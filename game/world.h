#include <vector>
#include <assert.h>
#include <algorithm>

#include "vec2.h"
#include "TileEngine.h"
#include "rect2.h"

static const int TILE_SIZE = 32;
static const double TOP_MIN = 300.0;

class World
{
public:

    World(engine::GamePtr game, engine::TileEnginePtr tileEngine) : 
        m_tileEngine(tileEngine),
        m_game(game)
    {
    }

    void update(int timeDelta)
    {

//        clearPreviousBlocks();

        m_position.x -= timeDelta / 2.0;

        if (shouldGenerateNewBlock())
        {
            generateNewBlock();
        }
    }

    void draw()
    {
        // draw blocks
        for (auto block : blocks)
        {
            drawBlock(block);
        }

        // draw runner
    }

    void drawBlock(const engine::Rect2& block);

    void createBlock(int left, int right, int top) 
    {
        engine::Rect2 newBlock(left, right, top, 800);

        blocks.push_back(newBlock);
        m_game->getPhysics().addStaticBody(newBlock);
    }

private:

    bool shouldGenerateNewBlock()
    {
        static const int offset = 50;

        engine::Rect2 lastBlock = blocks[blocks.size() - 1];
        return lastBlock.x + lastBlock.w + m_position.x < m_game->getScreenWidth() + 50;
    }

    void generateNewBlock()
    {
        engine::Random& random = m_game->getRandom();
        engine::Rect2 lastBlock = blocks[blocks.size() - 1];

        double left = lastBlock.x + lastBlock.w;

        int r1 = random.nextInt(0, 100);
        if (r1 > 30)
        {
            r1 = random.nextInt(100, 150);
            assert(r1 >= 100);

            left += random.nextInt(100, 150);
        }

        double top = max(TOP_MIN, lastBlock.y + random.nextInt(6) * 20 - 60);
        double width = random.nextInt(6, 12) * TILE_SIZE;

        engine::Rect2 newBlock{left, top, width, 800.0};

        blocks.push_back(newBlock);
    }

private:
    engine::Vec2 m_position;

    engine::GamePtr m_game;

    engine::TileEnginePtr m_tileEngine;

    std::vector<engine::Rect2> blocks;
};

inline void World::drawBlock(engine::Rect2 const& block)
{
    // Skip invisible blocks
    if (block.x + block.w + m_position.x < 0) return;

    double screenX = block.x + m_position.x;

    for (int i = 0; i < block.w / TILE_SIZE; ++i)
    {
        m_tileEngine->drawTile(screenX + i * TILE_SIZE, block.y, engine::GRASS_TOP);
    }

    for (int i = 0; i < block.w / TILE_SIZE; ++i)
    {
        for (int j = 1; j < 100; ++j)
        {
            if (j * TILE_SIZE + block.y > m_game->getScreenHeight()) break;
            m_tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE, engine::GROUND);
        }
    }
}