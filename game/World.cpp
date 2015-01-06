#include "World.h"

#include <Log.h>
#include <Memory.h>

using namespace engine;

static engine::Log _log{"World"};

World::World(Engine *engine, TileEnginePtr tileEngine) :
        tileEngine(tileEngine),
        engine(engine) {

    runner = std::make_unique<Runner>(this);
    runner->setPosition({160, 256 - runner->getSize().y});
    engine->registerUpdateable(runner.get());

    background = engine->getTextureFactory()->load("data/bg.png");
    backgroundFar = engine->getTextureFactory()->load("data/bg_far.png");
    backgroundMid = engine->getTextureFactory()->load("data/bg_mid.png");
    backgroundNear = engine->getTextureFactory()->load("data/bg_near.png");

    backgroundPainter.addTexture(background.get(), 0);
    backgroundPainter.addTexture(backgroundFar.get(), 0.25);
    backgroundPainter.addTexture(backgroundMid.get(), 0.41);
    backgroundPainter.addTexture(backgroundNear.get(), 0.83);

    sawTexture = engine->getTextureFactory()->load("data/saw.png");

    stepSoundSamples.push_back(engine->getSound()->loadSample("data/footstep.wav"));
    stepSoundSamples.push_back(engine->getSound()->loadSample("data/footstep2.wav"));

    runnerTrail = std::make_unique<RunnerTrail>(this);

    input = std::make_unique<StateInput>();
    engine->getInput()->addEventHandler(input->getEventHandler(), 1);

    createBlock(0, 2048, 256);

    engine->registerUpdateable(this);
    engine->registerRenderable(this);
};

World::~World() {
    engine->getInput()->removeEventHandler(input->getEventHandler());

    backgroundPainter.removeTexture(backgroundNear.get());
    backgroundPainter.removeTexture(backgroundMid.get());
    backgroundPainter.removeTexture(backgroundFar.get());
    backgroundPainter.removeTexture(background.get());

    // todo: unregister runner updateable

    // todo: unregister this updateable and drawable
}

void World::render(Renderer *renderer) {
    backgroundPainter.render(renderer, -position.x);

    drawBlocks();

    renderer->drawTexture(runnerTrail->getTrailTexture(), {0, 0});

    drawRunner();

    drawStats({10, 10});
}

void World::update(double timeDelta) {
    static Vec2 previousMousePosition;
    static bool dragWorld;

    if (input->mouseButtonIsDown(MouseButton::LEFT) && !dragWorld) {
        previousMousePosition = input->getMousePosition();
        dragWorld = true;
    }

    if (!input->mouseButtonIsDown(MouseButton::LEFT) && dragWorld) {
        dragWorld = false;
    }

    if (dragWorld) {
        Vec2 offset = previousMousePosition - input->getMousePosition();
        position += offset;
        previousMousePosition = input->getMousePosition();
    }

    if (input->keyIsDown(Key::SPACE)) {
        runner->addJumpForce();
    }

    if (runner->isInAir()) {
        stats.timeInAir += timeDelta;
    } else {
        stepPlayTime += timeDelta;
    }

    position.x = runner->getPosition().x - 160;

    runnerTrail->update(timeDelta);

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
    engine->getRenderer()->fillRect({runner->getPosition().x - position.x, runnerPosition.y - position.y, runnerSize.x, runnerSize.y});
}

inline void World::drawBlock(Rect2 const &block) {
    // Skip invisible blocks
    if (block.x + block.w - position.x < 0) return;

    double screenX = block.x - position.x;

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        tileEngine->drawTile(screenX + i * TILE_SIZE, block.y - position.y, GRASS_TOP);
    }

    for (int i = 0; i < block.w / TILE_SIZE; ++i) {
        for (int j = 1; j < 100; ++j) {
            if (j * TILE_SIZE + block.y - position.y > engine->getScreenHeight()) break;
            tileEngine->drawTile(screenX + i * TILE_SIZE, block.y + j * TILE_SIZE - position.y, GROUND);
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
    addBlock({left, top, right, 800});
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
