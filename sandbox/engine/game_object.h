#pragma once

#include <string>
#include <glm/glm.hpp>

#include "sprite.h"
#include "types.h"

namespace dsr {
    struct GameObject {
        std::string name;

        glm::vec2 size;

        bool isSolid;
        bool isDeadly;
        Sprite sprite;
    };
}