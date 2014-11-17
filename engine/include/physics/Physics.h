#include <vector>
#include <map>
#include <memory>

#include "physics/ParticleGeneratorRegistry.h"
#include "Vec2.h"
#include "Rect2.h"

namespace engine {
namespace physics {

class CollisionListener
{
public:
    virtual ~CollisionListener() = 0;
    virtual void onColision(int collisionSides) = 0;
};

enum class CollisionSide
{
    LEFT = 0x01,
    RIGHT = 0x02,
    TOP = 0x04,
    BOTTOM = 0x08
};

class Physics
{
public:
    Physics() : 
        m_gravity(Vec2(0, 0)) 
    {}

    ~Physics()
    {
        m_gravity = Vec2{0, 0};
    }

    void update(double timeDelta);

    void setGravity(const Vec2& gravity);

    // Adds a new static body that has no forces acting on it
    void addStaticBody(const Rect2& body);

    // Adds a new entity which is updated based on the forces which act upon it
    // Returns an ID for the entity
    int addEntity(const Rect2& entity);

    void setForce(int entityId, Vec2 force);

    Vec2 getEntityVelocity(int entityId);

    Rect2 getEntityBounds(int entityId);

private:
    void notifyCollision(int entityId, int collisionSides);

    Vec2 computeVelocity(int entityId);

    int computeCollisionSide(const Rect2& a, const Rect2& b);

    // Assuming a overlaps b,this method finds a new position for
    // a, such that it doesn't overlap with b.
    Rect2 resolveOverlap(const Rect2& a, const Rect2& b);

    Vec2 m_gravity;

    std::vector<Rect2> m_staticBodies;

    std::map<int, Rect2> m_entities;

    std::map<int, Vec2> m_forces;

    std::map<int, std::shared_ptr<CollisionListener>> m_listeners;

    ParticleGeneratorRegistry m_particleGeneratorRegistry;
};



inline void Physics::setGravity(const Vec2& gravity)
{
    m_gravity = gravity;
}

inline void Physics::addStaticBody(const Rect2& body)
{
    m_staticBodies.push_back(body);
}

inline int Physics::addEntity(const Rect2& entity)
{
    int entityCount = m_entities.size();
    m_entities[entityCount] = entity;

    return entityCount;
}

inline void Physics::setForce(int entityId, Vec2 force)
{
    m_forces[entityId] = force;
}

inline Vec2 Physics::getEntityVelocity(int entityId)
{
    return computeVelocity(entityId);   
}

inline Rect2 Physics::getEntityBounds(int entityId)
{
    return m_entities[entityId];
}

inline void Physics::notifyCollision(int entityId, int collisionSides)
{
    if (m_listeners.find(entityId) != m_listeners.end()) {
        m_listeners[entityId]->onColision(collisionSides);
    }
}

inline Vec2 Physics::computeVelocity(int entityId)
{
    return m_gravity + m_forces[entityId];
}

}
}