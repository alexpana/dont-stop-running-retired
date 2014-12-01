#include <iostream>

#include "Runner.h"
#include "World.h"

using namespace std;
using namespace engine;

void Runner::update(double delta) {

    // update velocity
    velocity.y += gravity * delta / 1000.0;

    // update position
    double distanceToObstacle = world->obstacleDistance(position + size);

    double distanceToGround = getDistanceToGround();

    double forwardMovement = min(distanceToObstacle, (delta / 1000.0) * velocity.x);

    position.x += forwardMovement;

    position.y += min(distanceToGround, (delta / 1000) * velocity.y);

    if (distanceToGround <= 1 && velocity.y > 0) {
        resetJump();
    }

    canJump = distanceToGround < 1.0;

    // update stats
    if (distanceToGround >= 1.0) {
        world->getStats().timeInAir += delta;
    }

    world->getStats().kilometersRan += forwardMovement;
}

void Runner::startJump() {
    if (!canJump) {
        return;
    }

    world->getStats().numberOfJumps += 1;

    velocity.y = shortJumpStartVelocity;
    isJumping = true;
    canJump = false;
    jumpTimer.restart();
}

void Runner::resetJump() {
    isJumping = false;
    velocity.y = 0;
}

void Runner::addJumpForce() {
    if (isJumping) {
        if (jumpTimer.getMilli() < 350) {
            velocity.y += -100 * (350 - jumpTimer.getMilli()) / 350.0;
        }
    } else {
        startJump();
    }
}

double Runner::getDistanceToGround() {
    Vec2 rightFoot = position + size;
    Vec2 leftFoot = position + Vec2{0, size.y};

    double rightFootFloorPosition = world->floorPosition(rightFoot);
    double leftFootFloorPosition = world->floorPosition(leftFoot);

    return std::min(rightFootFloorPosition - rightFoot.y, leftFootFloorPosition - leftFoot.y);
}

