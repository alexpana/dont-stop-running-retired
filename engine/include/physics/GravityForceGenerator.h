#include "physics/ParticleForceGenerator.h"

namespace game {
namespace physics {

class GravityForceGenerator : public engine::physics::ParticleForceGenerator
{
public:
    GravityForceGenerator()
    {

    }
    void updateForce(engine::physics::Particle& particle, double deltaTime) override
    {
        particle.addForce(particle.getMass() * )
    }

    ~GravityForceGenerator() {}

};



}
}