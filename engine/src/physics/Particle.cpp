#include "physics/Particle.h"

namespace engine {
namespace physics {
    Particle::Particle(double mass, Vec2 const& position, Vec2 const& velocity):
        m_inverseMass(1.0 / mass),
        m_position(position), 
        m_velocity(velocity),
        m_damping(0.999),
        m_acceleration(Vec2(0, 0))
    {
    }
}
}