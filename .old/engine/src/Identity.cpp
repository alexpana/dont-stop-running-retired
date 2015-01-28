#include "Identity.h"

namespace engine {

    std::string Identity::str() const {
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

    Identity Identity::generateRandom() {
        static engine::Random random;

        return {(((uint64_t) random.nextInt()) << 32) | random.nextInt(),
                (((uint64_t) random.nextInt()) << 32) | random.nextInt()};
    }

    bool operator==(const Identity &lhs, const Identity &rhs) {
        return lhs.low == rhs.low && lhs.high == rhs.high;
    }

    bool operator<(const Identity &lhs, const Identity &rhs) {
        if (lhs.high == rhs.high) {
            return lhs.low < rhs.low;
        } else {
            return lhs.high < rhs.high;
        }
    }
}