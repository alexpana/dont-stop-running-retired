#include "Physics.h"

#include <vector>

namespace engine {

    static const int MAX_COLLISION_SHAPES = 128;

    static CollisionShape *collisionShapePool = new CollisionShape[MAX_COLLISION_SHAPES];
    static bool *collisionShapePoolAvailability = new bool[MAX_COLLISION_SHAPES];

    bool circleContainsPoint(const Vec2 &circleOrigin, double circleRadius, const Vec2 &point) {
        return (circleOrigin - point).magnitude() < circleRadius;
    }

    bool circleRectIntersection(
            double circleOriginX, double circleOriginY, double circleRadius,
            double rectX, double rectY, double rectW, double rectH) {
        Vec2 circleOrigin = Vec2{circleOriginX, circleOriginY};
        Rect2 rect = Rect2{rectX, rectY, rectW, rectH};

        return circleContainsPoint(circleOrigin, circleRadius, rect.topLeft()) ||
                circleContainsPoint(circleOrigin, circleRadius, rect.topRight()) ||
                circleContainsPoint(circleOrigin, circleRadius, rect.bottomLeft()) ||
                circleContainsPoint(circleOrigin, circleRadius, rect.bottomRight());
    }

    bool circleCircleIntersection(double circleAOriginX, double circleAOriginY, double circleARadius,
            double circleBOriginX, double circleBOriginY, double circleBRadius) {
        Vec2 originA = Vec2{circleAOriginX, circleAOriginY};
        Vec2 originB = Vec2{circleBOriginX, circleBOriginY};

        return (originA - originB).magnitude() < circleARadius + circleBRadius;
    }

    bool rectRectIntersection(double rectAX, double rectAY, double rectAW, double rectAH,
            double rectBX, double rectBY, double rectBW, double rectBH) {
        Rect2 rectA = Rect2{rectAX, rectAY, rectAW, rectAH};
        Rect2 rectB = Rect2{rectBX, rectBY, rectBW, rectBH};
        return rectA.intersects(rectB);
    }

    bool PhysicsEngine::intersection(const CollisionShape &lhs, const ShapeTransformation &lhsTransform, const CollisionShape &rhs, const ShapeTransformation &rhsTransform) {
        if (lhs.type == CollisionShape::Type::CIRCLE) {
            if (rhs.type == CollisionShape::Type::CIRCLE) {
                // CIRCLE - CIRCLE
                return circleCircleIntersection(lhsTransform.translation.x, lhsTransform.translation.y, lhs.circle.radius,
                        rhsTransform.translation.x, rhsTransform.translation.y, rhs.circle.radius);
            } else if (rhs.type == CollisionShape::Type::RECT) {
                // CIRCLE - RECT
                return circleRectIntersection(lhsTransform.translation.x, lhsTransform.translation.y, lhs.circle.radius,
                        rhsTransform.translation.x - rhs.rect.width / 2, rhsTransform.translation.y - rhs.rect.height / 2, rhs.rect.width, rhs.rect.height);
            }
        } else if (lhs.type == CollisionShape::Type::RECT) {
            if (rhs.type == CollisionShape::Type::CIRCLE) {
                // RECT - CIRCLE
                return circleRectIntersection(rhsTransform.translation.x, rhsTransform.translation.y, rhs.circle.radius,
                        lhsTransform.translation.x - lhs.rect.width / 2, lhsTransform.translation.y - lhs.rect.height / 2, lhs.rect.width, lhs.rect.height);
            } else if (rhs.type == CollisionShape::Type::RECT) {
                // RECT - RECT
                return rectRectIntersection(lhsTransform.translation.x - lhs.rect.width / 2, lhsTransform.translation.y - lhs.rect.height / 2, lhs.rect.width, lhs.rect.height,
                        rhsTransform.translation.x - rhs.rect.width / 2, rhsTransform.translation.y - rhs.rect.height / 2, rhs.rect.width, rhs.rect.height);
            }
        }
        return false;
    }

    CollisionShape *PhysicsEngine::createCircleShape(double radius) {
        for (int i = 0; i < MAX_COLLISION_SHAPES; ++i) {
            if (collisionShapePoolAvailability[i]) {
                collisionShapePoolAvailability[i] = false;

                collisionShapePool[i].type = CollisionShape::Type::CIRCLE;
                collisionShapePool[i].circle.radius = radius;

                return &collisionShapePool[i];
            }
        }

        return nullptr;
    }

    CollisionShape *PhysicsEngine::createRectShape(double width, double height) {
        for (int i = 0; i < MAX_COLLISION_SHAPES; ++i) {
            if (collisionShapePoolAvailability[i]) {
                collisionShapePoolAvailability[i] = false;

                collisionShapePool[i].type = CollisionShape::Type::RECT;
                collisionShapePool[i].rect.width = width;
                collisionShapePool[i].rect.height = height;

                return &collisionShapePool[i];
            }
        }

        return nullptr;
    }

    void PhysicsEngine::releaseShape(CollisionShape *shape) {
        int i = (int) (shape - collisionShapePool);

        if (collisionShapePoolAvailability[i]) {
            // log warning: trying to free an unallocated shape
        }

        collisionShapePoolAvailability[i] = true;
    }
}