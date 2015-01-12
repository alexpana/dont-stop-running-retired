#pragma once

#include <functional>
#include <stdint.h>
#include <utility>

namespace engine {
    // 128-bit value UUID
    struct Identity {
        union {
            struct {
                uint64_t low;
                uint64_t high;
            };
        };

        Identity(uint64_t high, uint64_t low) : low(low), high(high) {
        };

        std::string toString() {
            static std::string digits = "0123456789abcdef";
            std::string result(36, '-');

            int strIndex = 0;

            for (int i = 15; i >= 0; --i) {
                result[strIndex++] = digits[(high >> (i * 4)) & 0xf];
                if (strIndex == 8) strIndex += 1;
                if (strIndex == 13) strIndex += 1;
            }

            strIndex += 1;

            for (int i = 15; i >= 0; --i) {
                result[strIndex++] = digits[(low >> (i * 4)) & 0xf];
                if (strIndex == 23) strIndex += 1;
            }

            return result;
        }
    };

    bool operator==(const Identity &lhs, const Identity &rhs);
    bool operator<(const Identity &lhs, const Identity &rhs);

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