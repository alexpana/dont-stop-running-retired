#include <iostream>

#include "Runner.h"
#include "World.h"

using namespace std;
using namespace engine;

void Runner::update(double delta) {
    m_velocity.y += m_gravity * delta / 1000.0;

    Vec2 rightFoot = m_position + m_size;
    Vec2 leftFoot = m_position + Vec2{0, m_size.y};

    double forwardSpace = m_world->obstacleDistance(rightFoot);

    double rightFootFloorPosition = m_world->floorPosition(rightFoot);
    double leftFootFloorPosition = m_world->floorPosition(leftFoot);

    double floorPosition = min(rightFootFloorPosition, leftFootFloorPosition);

    m_position.x += min(forwardSpace, (delta / 1000.0) * m_velocity.x);

    if (m_velocity.y > 0) {
        m_position.y = min(floorPosition - m_size.y, m_position.y + (delta / 1000) * m_velocity.y);
    } else {
        m_position.y += (delta / 1000) * m_velocity.y;
    }

    if (floorPosition - rightFoot.y <= 1 && m_velocity.y > 0) {
        endJump();
    }

}

void Runner::startJump() {
    if (!canJump) {
        return;
    }

    m_velocity.y = m_jumpStartVelocity;
    isJumping = true;
    canJump = false;
    m_jumpTimer.restart();
}

void Runner::endJump() {
    canJump = true;
    m_velocity.y = 0;
}