#include "World.h"

using namespace engine;

static engine::Log _log{"World"};

World::World(Engine *game, TileEnginePtr tileEngine) :
        tileEngine(tileEngine),
        engine(game) {
    runner = std::unique_ptr<Runner>{new Runner{this}};
    runner->setSpeed(0);

    game->registerUpdateable(runner.get());

    background = game->getTextureFactory()->loadTextureFromImage("data/bg.png");
    backgroundFar = game->getTextureFactory()->loadTextureFromImage("data/bg_far.png");
    backgroundMid = game->getTextureFactory()->loadTextureFromImage("data/bg_mid.png");
    backgroundNear = game->getTextureFactory()->loadTextureFromImage("data/bg_near.png");
    sawTexture = game->getTextureFactory()->loadTextureFromImage("data/saw.png");

    stepSoundSamples.push_back(game->getSound()->loadSample("data/footstep.wav"));
    stepSoundSamples.push_back(game->getSound()->loadSample("data/footstep2.wav"));

    game->getSound()->setSampleVolume(stepSoundSamples[0].get(), 1.0);
    game->getSound()->setSampleVolume(stepSoundSamples[1].get(), 1.0);

    // add initial block
    createBlock(0, 2048, 256);

    runner->setPosition(Vec2{160, 256 - runner->getSize().y});

    runnerTrail = std::unique_ptr<RunnerTrail>(new RunnerTrail{this});

    backgroundPainter.addTexture(background.get(), 0);
    backgroundPainter.addTexture(backgroundFar.get(), 0.25);
    backgroundPainter.addTexture(backgroundMid.get(), 0.41);
    backgroundPainter.addTexture(backgroundNear.get(), 0.83);
};

void World::render(Renderer *renderer) {
    backgroundPainter.render(renderer, -position.x);

    drawBlocks();

//    renderer->drawTexture(runnerTrail->getTrailTexture(), Vec2{0, 0});

    drawRunner();

    drawStats({10, 10});
}

void World::update(double timeDelta) {
    static Vec2 previousMousePosition;
    static bool dragWorld;

    if (engine->getInput()->mouseButtonIsDown(MouseButton::LEFT) && !dragWorld) {
        previousMousePosition = engine->getInput()->getMousePosition();
        dragWorld = true;
    }

    if (!engine->getInput()->mouseButtonIsDown(MouseButton::LEFT) && dragWorld) {
        dragWorld = false;
    }

    if (dragWorld) {
        Vec2 offset = previousMousePosition - engine->getInput()->getMousePosition();
        position += offset;
        previousMousePosition = engine->getInput()->getMousePosition();
    }

    if (engine->getInput()->keyIsDown(Key::Y)) {
        runner->addJumpForce();
    }

    if (runner->isInAir()) {
        stats.timeInAir += timeDelta;
    } else {
        stepPlayTime += timeDelta;
    }

//    position.x = runner->getPosition().x - 160;

//    runnerTrail->update(timeDelta);

    stats.metersRan = runner->getPosition().x / PIXELS_PER_METER;

#if 0
    if (stepPlayTime > stepPeriod) {
        stepPlayTime -= stepPeriod;
        stepPeriod = engine->getRandom()->nextInt(150, 180) / 1000.0;

        engine->getSound()->playSampleOnce(stepSoundSamples[currentStepSample].get());

        currentStepSample += 1;
        if (currentStepSample >= stepSoundSamples.size()) {
            currentStepSample = 0;
            std::random_shuffle(begin(stepSoundSamples), end(stepSoundSamples));
        }
    }
#endif

    if (shouldGenerateNewBlock()) {
        generateNewBlock();
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

    // render runner
    engine->getRenderer()->setColor(0x601030FF);
    engine->getRenderer()->fillRect(Rect2(runner->getPosition().x - position.x, runnerPosition.y, runnerSize.x, runnerSize.y));
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
            if (j * TILE_SIZE + block.y > engine->getScreenHeight()) break;
            tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE, GROUND);
        }
    }
}

void World::drawStats(const Vec2 &position) {
    std::stringstream stringStream;

    stringStream << "Number of jumps: " << stats.numberOfJumps << "\n";
    engine->getRenderer()->drawText(position, stringStream.str());

    stringStream.str("");
    stringStream << "Meters ran:  " << (int) (stats.metersRan * 10) / 10.0 << "\n";
    engine->getRenderer()->drawText(position + Vec2{0, 20}, stringStream.str());
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

Runner *World::getRunner() {
    return runner.get();
}

Stats &World::getStats() {
    return stats;
}

void World::displayConstants() {
//    std::cout << "Gravity:             " << runner->gravity << std::endl;
//    std::cout << "Jump start velocity: " << runner->longJumpStartVelocity << std::endl;
//    std::cout << "Speed:               " << runner->velocity.x << std::endl;
}

void World::generateNewBlock() {
    using namespace engine;

    Random *random = engine->getRandom();
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
    return lastBlock.x + lastBlock.w - position.x < engine->getScreenWidth() + 50;
}

void World::addBlock(const Rect2 &block) {
    blocks.push_back(block);

    blockOriginalY.push_back(block.y);

    blockTimeOffset.push_back(engine->getRandom()->nextInt(100) / 10.0);

    blockVelocity.push_back(0.005 + engine->getRandom()->nextInt(20) / 20000.0);
}

engine::Engine *World::getEngine() {
    return engine;
}

engine::Vec2 World::getCameraPosition() {
    return position;
}
