#pragma once

#include <cmath>
#include <random>

namespace engine {
    class Random {
    public:

        // C++ random generation mechanism uses a generating engine and a distribution
        Random() {
            std::random_device rd;
            randomEngine = std::default_random_engine(rd());
            uniformIntDistribution = std::uniform_int_distribution<int>(0, 0x00ffffff);
        }

        Random(const Random &other) = delete;

        // Generates the next integer value from 0 (inclusive) to max int (exclusive)
        int nextInt() {
            int randomNumber = uniformIntDistribution(randomEngine);
            return randomNumber;
        }

        // Generates the next integer value from 0 (inclusive) to n (exclusive)
        int nextInt(int n) {
            return nextInt() % n;
        }

        int nextInt(int from, int to) {
            return from + (nextInt() % (to - from));
        }

    private:
        std::default_random_engine randomEngine;
        std::uniform_int_distribution<int> uniformIntDistribution;
    };
}