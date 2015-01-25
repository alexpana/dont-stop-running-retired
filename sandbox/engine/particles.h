#pragma once

#include <functional>
#include <vector>

#include <bx/timer.h>

#include "settings.h"
#include "types.h"
#include "bgfx_utils.h"
#include "math.h"

namespace dsr {

    U32 lerpColor(U32 a, U32 b, F32 t);

    template<typename T>
    struct ParticleProperty {
        T start;
        T end;
        T current;

        std::function<T(const T &, const T &, F32)> interpolation = dsr::lerp<T>;

        void update(F32 t) {
            current = interpolation(start, end, t);
        }
    };

    struct Particle {
        F32 direction[3];

        ParticleProperty<F32> speed;
        ParticleProperty<F32> rotation;
        ParticleProperty<F32> scale;
        ParticleProperty<U32> color;
        ParticleProperty<F32> alpha;

        F32 position[3];

        I64 lifeTime;
        I64 currentLife;

        void update(I64 dt);
    };

    template<typename T>
    struct Range {
        T min;
        T max;

        std::function<T(T, T, F32)> distribution = dsr::lerp<T>;

        Range() : min(), max() {
        };

        Range(T min, T max) : min(min), max(max) {
        };

        T distribute(F32 t) {
            return distribution(min, max, t);
        }
    };

    class ParticleSystem;

    class ParticleGenerator {
    public:
        struct Params {;
            Range<I64> lifetime;

            Range<U32> startColor;
            Range<U32> endColor;

            Range<F32> startRotation;
            Range<F32> endRotation;

            Range<F32> startScale;
            Range<F32> endScale;

            Range<F32> startSpeed;
            Range<F32> endSpeed;

            Range<F32> startAlpha;
            Range<F32> endAlpha;
        } params;

        F32 generatorPosition[3];

        F32 generatorSpawnRadius;

        F32 generatorSpawnDirection[3];

        F32 generatorSpawnArc;

        // Particles per second
        F32 generatorSpawnFrequency;

        void update(I64 dt);

        ParticleGenerator();

        ParticleGenerator(ParticleGenerator&& other) = default;

        void setParentSystem(ParticleSystem *particleSystem) {
            parentSystem = particleSystem;
        }

    private:
        void spawnParticle();

        I64 lastSpawnTime;

        ParticleSystem *parentSystem = nullptr;
    };

    class ParticleSystem {
    public:
        ParticleSystem() = default;

        ~ParticleSystem() = default;

        void addGenerator(ParticleGenerator *generator);

        void removeGenerator(ParticleGenerator &generator);

        void update(I64 dt);

        void render();

        Particle *allocateParticle();

    private:
        std::vector<ParticleGenerator *> generators;
        const bgfx::InstanceDataBuffer *instanceDataBuffer;

        static Particle sParticlePool[dsr::kParticleLimit];

        void fillInstanceBuffer();
    };
}
