#include "Identity.h"

namespace engine {

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