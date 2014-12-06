#include "RunnerTrail.h"

#include "World.h"

using namespace engine;

RunnerTrail::RunnerTrail(World *world) : world(world) {
    Game *game = world->getGame();
    frontBuffer = game->getRenderer()->createTexture(game->getScreenWidth(), game->getScreenHeight(), SDL_TEXTUREACCESS_TARGET);
    backBuffer = game->getRenderer()->createTexture(game->getScreenWidth(), game->getScreenHeight(), SDL_TEXTUREACCESS_TARGET);
    trailGradient = game->getTextureFactory()->loadImage("data/trail.png");
}

void RunnerTrail::update(double /*deltaTime*/) {
    Game *game = world->getGame();
    Runner *runner = world->getRunner();

    if (!trailInitialized) {
        previousRunnerPosition = runner->getPosition();
        trailInitialized = true;
        return;
    }

    Vec2 runnerPosition = runner->getPosition();

    game->getRenderer()->setAlphaModulation(frontBuffer.get(), 0xFF);
    game->getRenderer()->setAlphaModulation(backBuffer.get(), 0xFF);

    std::swap(frontBuffer, backBuffer);


    game->getRenderer()->setTarget(frontBuffer.get());

    game->getRenderer()->setColor(0x00000000);
    game->getRenderer()->clear();

    double deltaX = runnerPosition.x - previousRunnerPosition.x;
    double deltaY = runnerPosition.y - previousRunnerPosition.y;

    game->getRenderer()->setColor(0xFF0000FF);

    if (!(deltaX < 10 && deltaY < -20)) {
        for (int i = 1; i <= deltaX + 1; ++i) {
            int interpolatedX = (int) (previousRunnerPosition.x + i);
            int interpolatedY = (int) (previousRunnerPosition.y + deltaY * (double) (i - 1) / deltaX);

            drawTrailSegment(interpolatedX, interpolatedY);
        }
    }

    game->getRenderer()->drawTexture(backBuffer.get(), Vec2{-deltaX, 0});

    game->getRenderer()->resetTarget();

    game->getRenderer()->setAlphaModulation(frontBuffer.get(), 0x90);

    previousRunnerPosition = runnerPosition;
}

void RunnerTrail::drawTrailSegment(int interpolatedX, int interpolatedY) {
    Vec2 position = world->getCameraPosition();
    world->getGame()->getRenderer()->drawTexture(trailGradient.get(),
            Vec2{interpolatedX - position.x, interpolatedY - position.y});
}

engine::Texture *RunnerTrail::getTrailTexture() {
    return frontBuffer.get();
}
