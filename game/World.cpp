#include "World.h"

using namespace engine;

World::World(GamePtr game, TileEnginePtr tileEngine) :
        tileEngine(tileEngine),
        game(game),
        logger("Game") {
    runner = std::make_shared<Runner>(this);

    game->registerUpdateable(runner);

    background = game->getTextureFactory()->loadImage("data/bg.png");
    backgroundFar = game->getTextureFactory()->loadImage("data/bg_far.png");
    backgroundMid = game->getTextureFactory()->loadImage("data/bg_mid.png");
    backgroundNear = game->getTextureFactory()->loadImage("data/bg_near.png");

    stepSample1 = game->getSound()->loadSample("data/footstep.wav");
    stepSample2 = game->getSound()->loadSample("data/footstep2.wav");

    game->getSound()->setSampleVolume(stepSample1.get(), 0.2);
    game->getSound()->setSampleVolume(stepSample2.get(), 0.2);

    trailFrontBuffer = game->getRenderer()->createTexture(800, 600, SDL_TEXTUREACCESS_TARGET);
    trailBackBuffer = game->getRenderer()->createTexture(800, 600, SDL_TEXTUREACCESS_TARGET);

}

void World::updateTrail() {
    if (!trailInitialized) {
        previousRunnerPosition = runner->getPosition();
        trailInitialized = true;
        return;
    }

    Vec2 runnerPosition = runner->getPosition();

    game->getRenderer()->setAlphaModulation(trailFrontBuffer.get(), 0xFF);
    game->getRenderer()->setAlphaModulation(trailBackBuffer.get(), 0xFF);

    std::swap(trailFrontBuffer, trailBackBuffer);


    game->getRenderer()->setTarget(trailFrontBuffer.get());

    game->getRenderer()->setColor(0x00000000);
    game->getRenderer()->clear();

    double deltaX = runnerPosition.x - previousRunnerPosition.x;
    double deltaY = runnerPosition.y - previousRunnerPosition.y;

    game->getRenderer()->setColor(0xFF0000FF);

    if (!(deltaX < 10 && deltaY < -20)) {
        for (int i = 1; i <= deltaX + 1; ++i) {
            int interpolatedX = (int) (previousRunnerPosition.x + i);
            int interpolatedY = (int) (previousRunnerPosition.y + deltaY * (double) (i - 1) / deltaX);

            int runnerHeight = (int) runner->getSize().y;

            double heightStep = runnerHeight / 6.0;

            static int colors[] = {0xff0000ff, 0xff6600ff, 0xffff00ff, 0x00cc00ff, 0x3366ffff, 0x6600ccff};

            for (int j = 0; j < 6; ++j) {
                game->getRenderer()->setColor(colors[j]);

                game->getRenderer()->drawLine(
                        Vec2{interpolatedX - position.x, interpolatedY - position.y + j * heightStep},
                        Vec2{interpolatedX - position.x, interpolatedY - position.y + std::min((double) runnerHeight - 1, (j + 1) * heightStep)});
            }
        }
    }

    game->getRenderer()->drawTexture(trailBackBuffer.get(), Vec2{-deltaX, 0});

    game->getRenderer()->resetTarget();

    game->getRenderer()->setAlphaModulation(trailFrontBuffer.get(), 0x90);

    previousRunnerPosition = runnerPosition;
}

void World::draw() {
    drawBackground();

    drawBlocks();

    game->getRenderer()->drawTexture(trailFrontBuffer.get(), Vec2{0, 0});

    drawRunner();

    drawStats({10, 10});
}

void World::update(double timeDelta) {
    if (runner->isInAir()) {
        stats.timeInAir += timeDelta;
    } else {
        stepPlayTime += timeDelta;
    }

    position.x = runner->getPosition().x - 160;

    updateTrail();

    stats.kilometersRan = runner->getPosition().x / PIXELS_PER_METER / 1000.0;

    if (stepPlayTime > stepPeriod) {
        stepPlayTime -= stepPeriod;
        stepPeriod = game->getRandom()->nextInt(150, 180);
        game->getSound()->playSampleOnce(game->getRandom()->nextInt(0, 100) > 20 ? stepSample1.get() : stepSample2.get());
    }

    if (shouldGenerateNewBlock()) {
        generateNewBlock();
    }

    // move blocks
//    for (int i = 0; i < blocks.size(); ++i) {
//        long double delta = std::sin(blockTimeOffset[i]) * 0.3;
//        blocks[i].y += delta;
//        blockTimeOffset[i] += blockVelocity[i];
//    }
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
    game->getRenderer()->fillRect(Rect2(runner->getPosition().x - position.x, runnerPosition.y, runnerSize.x, runnerSize.y));
}

void World::drawBackground() {
    static const double BG_FAR_SPEED = 4.0;
    static const double BG_MID_SPEED = 2.4;
    static const double BG_NEAR_SPEED = 1.2;

    game->getRenderer()->drawTexture(background.get(), Vec2{0, 0});

    drawTilingBackgroundTexture(backgroundFar.get(), -position.x / BG_FAR_SPEED);
    drawTilingBackgroundTexture(backgroundMid.get(), -position.x / BG_MID_SPEED);
    drawTilingBackgroundTexture(backgroundNear.get(), -position.x / BG_NEAR_SPEED);
}

inline void World::drawBlock(Rect2 const &block) {
    // Skip invisible blocks
    if (block.x + block.w - position.x < 0) return;

    double screenX = block.x - position.x;

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
    addBlock(Rect2(left, top, right, 800));
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

    Random *random = game->getRandom();
    Rect2 lastBlock = blocks[blocks.size() - 1];
    double lastBlockOriginalHeight = blockOriginalY[blocks.size() - 1];

    double left = lastBlock.x + lastBlock.w;

    int r1 = random->nextInt(0, 100);
    if (r1 > 30) {
        left += random->nextInt(100, 150);
    }

    double top = std::max(TOP_MIN, lastBlockOriginalHeight + random->nextInt(6) * 20 - 60);
    double width = random->nextInt(6, 12) * TILE_SIZE;

    addBlock(Rect2{left, top, width, 100.0});
}

bool World::shouldGenerateNewBlock() {
    Rect2 lastBlock = blocks[blocks.size() - 1];
    return lastBlock.x + lastBlock.w - position.x < game->getScreenWidth() + 50;
}

void World::addBlock(const Rect2 &block) {
    blocks.push_back(block);

    blockOriginalY.push_back(block.y);

    blockTimeOffset.push_back(game->getRandom()->nextInt(100) / 10.0);

    blockVelocity.push_back(0.005 + game->getRandom()->nextInt(20) / 20000.0);
}

void World::drawTilingBackgroundTexture(Texture *texture, double offset) {
    int roundedX = (int) offset % texture->getWidth();

    if (roundedX > 0) {
        roundedX -= texture->getWidth();
    }

    Rect2 src{std::abs(roundedX), 0,
            std::min(game->getScreenWidth(), texture->getWidth() + roundedX),
            texture->getHeight()};
    Vec2 dst{0, 0};

    while (dst.x < game->getScreenWidth()) {
        game->getRenderer()->drawTexture(texture, dst, src);
        dst.x += src.w;

        src.x = (int)(src.x + src.w) % texture->getWidth();
        src.w = std::min((int)(game->getScreenWidth() - dst.x), texture->getWidth());
    }
}
