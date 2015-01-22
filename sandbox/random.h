#pragma once

#include <random>

namespace dsr {

    template<typename Engine, typename Distribution>
    class RNG {
    public:
        RNG() {
            std::random_device rd;
            engine = Engine(rd());
            distribution = Distribution(0, 0x00ffffff);
        }

        RNG(const RNG &other) = delete;

        typename Distribution::result_type gen() {
            return distribution(engine);
        }

    private:
        Engine engine;
        Distribution distribution;
    };

    typedef RNG<std::default_random_engine, std::uniform_int_distribution<U32>> U32RNG;

    typedef RNG<std::default_random_engine, std::uniform_real_distribution<F32>> F32RNG;
}