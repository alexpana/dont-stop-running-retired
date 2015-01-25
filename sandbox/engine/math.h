#pragma once

#include <bx/fpumath.h>
#include <cmath>

#include "types.h"

namespace dsr {
    using namespace bx;

    template<typename T>
    T lerp(const T &a, const T &b, F32 t) {
        return (T) (a + (b - a) * t);
    }

    static void vec3Rot2(float *dest, float *source, float angle) {
        F32 s = std::sin(angle);
        F32 c = std::cos(angle);
        dest[0] = source[0] * c - s * source[1];
        dest[1] = source[1] * c + s * source[0];
    }
}