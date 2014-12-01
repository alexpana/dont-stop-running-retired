#include "World.h"

using namespace engine;

World::World(GamePtr game, TileEnginePtr tileEngine) :
        tileEngine(tileEngine),
        game(game) {
    runner = std::make_shared<Runner>(this);
    game->registerUpdateable(runner);

    background = game->getTextureFactory().loadImage("data/bg.png");
}

void World::draw() {
    drawBackground();

    drawBlocks();

    drawRunner();

    drawStats({10, 10});
}

void World::update(double) {
    position.x = -runner->getPosition().x + 60;
    stats.kilometersRan = runner->getPosition().x / PIXELS_PER_METER / 1000.0;

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

void World::drawBlocks() {
    for (const auto &block : blocks) {
        drawBlock(block);
    }
}

void World::drawRunner() {
    auto runnerPosition = runner->getPosition();
    auto runnerSize = runner->getSize();

    // draw runner
    game->getRenderer()->setColor(0x601030FF);
    game->getRenderer()->fillRect(Rect2(60.0, runnerPosition.y, runnerSize.x, runnerSize.y));
}

void World::drawBackground() {
    game->getRenderer()->drawTexture(background, Vec2((int) position.x % background->getWidth(), 0));
    game->getRenderer()->drawTexture(background, Vec2((int) position.x % background->getWidth() + background->getWidth(), 0));
}

inline void World::drawBlock(Rect2 const &block) {
    // Skip invisible blocks
    if (block.x + block.w + position.x < 0) return;

    double screenX = block.x + position.x;

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        tileEngine->drawTile(screenX + i * TILE_SIZE, block.y, GRASS_TOP);
    }

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j * TILE_SIZE + block.y > game->getScreenHeight()) break;
            tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE, GROUND);
        }
    }
}

void World::drawStats(const Vec2 &position) {
    std::stringstream stringStream;

    stringStream << "Number of jumps: " << stats.numberOfJumps << "\n";
    game->getRenderer()->drawText(position, stringStream.str());

    stringStream.str("");
    stringStream << "Kilometers ran:  " << stats.kilometersRan << "\n";
    game->getRenderer()->drawText(position + Vec2{0, 20}, stringStream.str());
}

void World::createBlock(int left, int right, int top) {
    addBlock(Rect2(left, right, top, 800));
}

double World::obstacleDistance(Vec2 position) {
    double minDistance = 1000;

    for (auto &block : blocks) {
        if (block.topLeft().y < position.y && block.topLeft().x > position.x) {
            minDistance = std::min(minDistance, block.topLeft().x - position.x);
        }
    }

    return minDistance;
}

double World::floorPosition(Vec2 position) {
    for (auto &block : blocks) {
        if (block.topLeft().x < position.x && block.topRight().x > position.x) {
            return block.topLeft().y;
        }
    }

    return 1000;
}

RunnerPtr World::getRunner() {
    return runner;
}

Stats &World::getStats() {
    return stats;
}

void World::displayConstants() {
    std::cout << "Gravity:             " << runner->gravity << std::endl;
    std::cout << "Jump start velocity: " << runner->longJumpStartVelocity << std::endl;
    std::cout << "Speed:               " << runner->velocity.x << std::endl;
}

void World::generateNewBlock() {
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

    addBlock(Rect2{left, top, width, 100.0});
}

bool World::shouldGenerateNewBlock() {
    Rect2 lastBlock = blocks[blocks.size() - 1];
    return lastBlock.x + lastBlock.w + position.x < game->getScreenWidth() + 50;
}

void World::addBlock(const Rect2 &block) {
    blocks.push_back(block);

    blockOriginalY.push_back(block.y);

    blockTimeOffset.push_back(game->getRandom().nextInt(100) / 10.0);

    blockVelocity.push_back(0.005 + game->getRandom().nextInt(20) / 20000.0);
}