#pragma once

#include <numeric>
#include <vector>

#include "Vec2.h"

namespace engine {
namespace physics {

class Particle 
{
public:
    Particle(double mass, 
        const Vec2& position, 
        const Vec2& velocity = Vec2{0, 0});

    void addForce(const Vec2& force);

    void setMass(double mass);

    void setInverseMass(double mass);

    double getMass();

    bool hasFiniteMass();

    void update(double deltaTime);

private:
    Vec2 m_position;
    Vec2 m_velocity;
    Vec2 m_acceleration;

    // Used to add together the forces acting on the object during the current update
    Vec2 m_forceAccumulator;

    // Used both for drag (air friction) and for correcting floating point errors
    double m_damping;

    // Used instead of the mass, because it can more easily represent infinite
    // mass, but never zero mass.
    double m_inverseMass;
};

inline void Particle::addForce(const Vec2&  force)
{
    m_forceAccumulator += force;
}

inline void Particle::setMass(double mass)
{
    m_inverseMass = mass;
}

inline void Particle::setInverseMass(double mass)
{
    m_inverseMass = 1.0 / mass;
}

inline double Particle::getMass() 
{
    return 1.0 / m_inverseMass;
}

inline bool Particle::hasFiniteMass()
{
    return m_inverseMass > 0;
}

inline void Particle::update(double deltaTime)
{
    // Update position
    m_position += m_velocity * deltaTime;

    // Update velocity
    m_acceleration = m_inverseMass * m_forceAccumulator;

    m_velocity += m_acceleration * deltaTime;
    m_velocity *= pow(m_damping, deltaTime);

    // Clear force accumulator
    m_forceAccumulator = Vec2(0, 0);
}

}
}