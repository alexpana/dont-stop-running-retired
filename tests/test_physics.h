#pragma once

#include "fructose/fructose.h"

#include "physics/Physics.h"
#include "physics/CollisionSystem.h"


class TestPysics : public fructose::test_base<TestPysics> {
public:
    void testGravitySimple(const std::string& test_name) 
    {
        using namespace engine;

        physics::Physics physics;
        physics.setGravity(engine::Vec2(0, 1));

        Vec2 entityPosition(0, 0);

        int entityId = physics.addEntity(engine::Rect2(0, 0, 10, 10));

        physics.update(1);
        entityPosition = physics.getEntityBounds(entityId).topLeft();
        fructose_assert(entityPosition == Vec2(0, 1));

        physics.update(2.5);
        entityPosition = physics.getEntityBounds(entityId).topLeft();
        fructose_assert(entityPosition == Vec2(0, 3.5));

        physics.update(-1);
        entityPosition = physics.getEntityBounds(entityId).topLeft();
        fructose_assert(entityPosition == Vec2(0, 2.5));

        physics.setGravity(Vec2(-2, 0));
        physics.update(0.5);
        entityPosition = physics.getEntityBounds(entityId).topLeft();
        fructose_assert(entityPosition == Vec2(-1, 2.5));
    }

    void testVelocity(const std::string& test_name) 
    {
        using namespace engine;

        physics::Physics physics;
        physics.setGravity(Vec2(0, 1));

        Vec2 entityPosition(0, 0);

        int entityId = physics.addEntity(Rect2(0, 0, 10, 10));
    }

    void testRayLineCollision(const std::string& test_name)
    {
        using namespace engine;

        physics::CollisionDetector collisionDetector;
        physics::CollisionDetector::Result result(false);

        result = collisionDetector.rayIntersection(Vec2(0, 0), Vec2(0, 1), Vec2(-2, -2), Vec2(0, 0));
        fructose_assert(result.hasIntersection);
        fructose_assert(result.intersection == Vec2(0,0));

        result = collisionDetector.rayIntersection(Vec2(6, 5), Vec2(-2, -1), Vec2(0, 4), Vec2(4, 2));
        fructose_assert(result.hasIntersection);
        fructose_assert(result.intersection == Vec2(2, 3));

        // not parallel but not intersecting
        result = collisionDetector.rayIntersection(Vec2(2, 1), Vec2(3, 1), Vec2(1, 1), Vec2(3, 4));
        fructose_assert(result.hasIntersection == false);

        result = collisionDetector.rayIntersection(Vec2(5, 2), Vec2(-3, -1), Vec2(1, 1), Vec2(3, 4));
        fructose_assert(result.hasIntersection == false);

        // opposite direction
        result = collisionDetector.rayIntersection(Vec2(2, 2), Vec2(1, 1), Vec2(3, 2), Vec2(1, 1));
        fructose_assert(result.hasIntersection == false);

        // parallel
        result = collisionDetector.rayIntersection(Vec2(2, 2), Vec2(1, 1), Vec2(3, 2), Vec2(6, 5));
        fructose_assert(result.hasIntersection == false);

        // segment end
        result = collisionDetector.rayIntersection(Vec2(2, 2), Vec2(-1, -1), Vec2(0, 0), Vec2(-1, 4));
        fructose_assert(result.hasIntersection);
        fructose_assert(result.intersection == Vec2(0, 0));
    }

    void testRayPolygonCollision(const std::string& test_name)
    {
        using namespace engine;

        physics::CollisionDetector collisionDetector;
        physics::CollisionDetector::Result result(false);
    }
};