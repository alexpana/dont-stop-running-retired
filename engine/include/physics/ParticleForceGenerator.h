#include "physics/Particle.h"

namespace engine {
namespace physics {

class ParticleForceGenerator
{
public:
    virtual void updateForce(Particle& particle, double deltaTime) = 0;

    virtual ~ParticleForceGenerator() = 0;
};

}
}