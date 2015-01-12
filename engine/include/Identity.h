#pragma once

#include <functional>
#include <stdint.h>
#include <utility>
#include "Random.h"

namespace engine {

    // 128-bit value UUID
    struct Identity {
        union {
            struct {
                uint64_t low;
                uint64_t high;
            };
        };

        Identity(const Identity &other) : low(other.low), high(other.high) {
        }

        Identity(uint64_t high, uint64_t low) : low(low), high(high) {
        };

        std::string str() const;

        static Identity generateRandom();

    };

    bool operator==(const Identity &lhs, const Identity &rhs);

    bool operator<(const Identity &lhs, const Identity &rhs);

    // included to provide templates for the other operators
    using namespace std::rel_ops;
}

namespace std {
    template<>
    struct hash<engine::Identity> {
        std::size_t operator()(const engine::Identity &identity) const {
            return hash<uint64_t>{}(identity.high) ^ (hash<uint64_t>{}(identity.low) << 1);
        }
    };
}