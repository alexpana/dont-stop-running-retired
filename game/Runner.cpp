#include <iostream>

#include "Runner.h"
#include "World.h"

using namespace std;
using namespace engine;

void Runner::update(double delta) {
    velocity.y += gravity * delta / 1000.0;

    Vec2 rightFoot = position + size;
    Vec2 leftFoot = position + Vec2{0, size.y};

    double rightFootFloorPosition = world->floorPosition(rightFoot);
    double leftFootFloorPosition = world->floorPosition(leftFoot);

    double forwardSpace = world->obstacleDistance(rightFoot);

    double floorPosition = min(rightFootFloorPosition, leftFootFloorPosition);

    position.x += min(forwardSpace, (delta / 1000.0) * velocity.x);

    if (velocity.y > 0) {
        // when falling, make sure we don't go through the floor
        position.y = min(floorPosition - size.y,
                position.y + (delta / 1000) * velocity.y);

    } else {
        // the sky is the limit
        position.y += (delta / 1000) * velocity.y;
    }

    if (floorPosition - rightFoot.y <= 1 && velocity.y > 0) {
        resetJump();
    }

    canJump = floorPosition - rightFoot.y <= 1;
}

void Runner::startJump() {
    if (!canJump) {
        return;
    }

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
        if (jumpTimer.getMilli() < 300) {
            velocity.y += -100 * (300 - jumpTimer.getMilli()) / 300.0;
        }
    } else {
        startJump();
    }
}
