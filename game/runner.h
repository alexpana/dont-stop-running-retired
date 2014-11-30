#pragma once

#include <memory>

#include "Defines.h"

#include <Game.h>
#include <Vec2.h>
#include <Timer.h>

class World;

class Runner : public engine::Game::IUpdateable {
public:
    explicit Runner(World *world) :
            m_size(20, 40),
            isJumping(false),
            m_world(world) {
        m_velocity.x = 410; // 20km / s
    }

    void update(double delta) override;

    engine::Vec2 getPosition() const {
        return m_position;
    }

    engine::Vec2 getSize() const {
        return m_size;
    }

    void startJump();

    void endJump();

    void increaseSpeed() {
        m_velocity.x += 2.0;
    }

    void decreaseSpeed() {
        m_velocity.x -= 2.0;
    }

    void increaseGravity() {
        m_gravity += 10.0;
    }

    void decreaseGravity() {
        m_gravity -= 10.0;
    }

    void increaseJumpVelocity() {
        m_jumpStartVelocity += -10.0;
    }

    void decreaseJumpVelocity() {
        m_jumpStartVelocity -= -10.0;
    }

    void setSpeed(double speed) {
        m_velocity.x = speed;
    }

    void setGravity(double gravity) {
        this->m_gravity = gravity;
    }

    void setJumpVelocity(double velocity) {
        m_jumpStartVelocity = velocity;
    }

public:
    double m_gravity = 1000.0;

    double m_jumpStartVelocity = -400.0;

    engine::Vec2 m_velocity;

private:
    bool canJump = true;

    bool isJumping = false;

    engine::Timer m_jumpTimer;

    World *m_world;

    engine::Vec2 m_position;

    engine::Vec2 m_size;
};

typedef std::shared_ptr<Runner> RunnerPtr;