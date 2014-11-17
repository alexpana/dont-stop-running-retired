#include <algorithm>
#include <map>

#include "physics/Particle.h"
#include "physics/ParticleForceGenerator.h"

namespace engine {
namespace physics {

class ParticleGeneratorRegistry
{
public:
    void addGenerator(const Particle& particle, const ParticleForceGenerator& generator);

    void removeGenerator(const Particle& particle, const ParticleForceGenerator& generator);

    void clear(const Particle& particle);

    const std::vector<const ParticleForceGenerator*>& getGenerators(const Particle& particle);

private:
    void ensureInitialized(const Particle& particle);

    std::map<const Particle*, std::vector<const ParticleForceGenerator*>> m_forceGenerators;
};

inline void ParticleGeneratorRegistry::addGenerator(const Particle& particle, const ParticleForceGenerator& generator)
{
    ensureInitialized(particle);

    m_forceGenerators[&particle].push_back(&generator);
}

inline void ParticleGeneratorRegistry::removeGenerator(const Particle& particle, const ParticleForceGenerator& generator)
{
    using namespace std;

    ensureInitialized(particle);

    auto generators = m_forceGenerators[&particle];

    m_forceGenerators[&particle].erase(remove_if(
        begin(generators), 
        end(generators), 
        [&generator](const ParticleForceGenerator* g){ return g == &generator; }));
}

inline void ParticleGeneratorRegistry::clear(const Particle& particle)
{
    m_forceGenerators.erase(&particle);
}

inline const std::vector<const ParticleForceGenerator*>& ParticleGeneratorRegistry::getGenerators(const Particle& particle)
{
    ensureInitialized(particle);
    return m_forceGenerators[&particle];
}

inline void ParticleGeneratorRegistry::ensureInitialized(const Particle& particle)
{
    if (m_forceGenerators.count(&particle) == 0) {
        m_forceGenerators[&particle] = std::vector<const ParticleForceGenerator*>(1);
    }
}

}
}