#pragma once

#include <cmath>
#include <random>

namespace engine {

class Random
{
public:
    // C++ random generation mechanism uses a generating engine
    // and a distribution
    Random()
    {
        std::random_device rd;
        m_randomEngine = std::mt19937(rd());
        m_uniformIntDistribution = std::uniform_int_distribution<int>();
    }

    // Generates the next integer value from 0 (inclusive) to max int (exclusive)
    int nextInt()
    {
        return abs(static_cast<int>(m_randomEngine()));
    }

    // Generates the next integer value from 0 (inclusive) to n (exclusive)
    int nextInt(int n)
    {
        return nextInt() % n;
    }

    int nextInt(int from, int to)
    {
        return from + (nextInt() % (to - from));
    }

private:
    std::mt19937 m_randomEngine;
    std::uniform_int_distribution<int> m_uniformIntDistribution;
};

}