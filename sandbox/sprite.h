#pragma once

#include <glm/glm.hpp>

#include "types.h"

namespace dsr {
    struct Sprite {
        std::string sheetName;
        glm::vec2 offset;
        glm::vec2 size;
        glm::vec2 origin;
    };
}