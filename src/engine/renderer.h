#pragma once

#include <glm/vec2.hpp>
#include "types.h"

namespace dsr {
    class Sprite;

    void renderSprite(const glm::vec2& position, const Sprite& sprite, const U32 depth = 0);
    void renderSprite(const Sprite& sprite, const U32 depth = 0);
}