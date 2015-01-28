#pragma once

#include "Vec2.h"

namespace engine {
    class Transform {
    public:
        Vec2 position;
        float rotation;
        float scale;
    };
}