#include "physics/Physics.h"

namespace engine {
namespace physics {


void Physics::update(double timeDelta)
{
    for (auto& entity : m_entities)
    {
        // move entity
        Vec2 velocity = computeVelocity(entity.first);
        Rect2 newBounds = entity.second.translated(velocity * timeDelta);

        // handle collision
        for (auto& body : m_staticBodies)
        {
            if (newBounds.intersects(body))
            {
                auto sides = computeCollisionSide(newBounds, body);
                notifyCollision(entity.first, sides);


            }
        }

        // update position
        entity.second = newBounds;
    }
}

int Physics::computeCollisionSide(Rect2 const& a, Rect2 const& b)
{
    int collisionSides = 0;

    if (b.contains(a.bottomLeft()) && b.contains(a.bottomRight())) collisionSides += static_cast<int>(CollisionSide::BOTTOM);
    if (b.contains(a.topRight()) && b.contains(a.bottomRight())) collisionSides += static_cast<int>(CollisionSide::RIGHT);
    if (b.contains(a.topLeft()) && b.contains(a.topRight())) collisionSides += static_cast<int>(CollisionSide::TOP);
    if (b.contains(a.topLeft()) && b.contains(a.bottomLeft())) collisionSides += static_cast<int>(CollisionSide::LEFT);

    return collisionSides;
}

Rect2 Physics::resolveOverlap(Rect2 const& a, Rect2 const& b)
{
    Rect2 result(a);
    double ny, r, nx;

    Vec2 v = a.centerOfMass() - b.centerOfMass();
    if (v.x < v.y)
    {
        ny = (a.h + b.h) / 2;
        r = ny / v.y;
        nx = v.x * r;
    } else
    {
        nx = (a.w + b.w) / 2;
        r = nx / v.x;
        ny = v.y * r;
    }

    result.x += nx - a.x;
    result.y += ny - a.y;            

    return result;
}

}
}