#pragma once

#include <functional>
#include <stdint.h>

namespace engine {
    struct Identity {
        union {
            struct {
                uint16_t low;
                uint16_t high;
            };
            uint32_t value;
        };

        Identity(uint16_t low, uint16_t high) : low(low), high(high) {
        };

        bool operator==(const Identity &rhs) const {
            return value == rhs.value;
        }
    };
}

namespace std {
    template<>
    struct hash<engine::Identity> {
        std::size_t operator()(const engine::Identity &identity) const {
            return hash<uint32_t>{}(identity.value);
        }
    };

    template<>
    struct less<engine::Identity> {
        bool operator()(const engine::Identity &lhs, const engine::Identity &rhs) const {
            return lhs.value < rhs.value;
        }
    };
}