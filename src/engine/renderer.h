#pragma once

#include <glm/vec2.hpp>
#include "types.h"

namespace dsr {
    class Sprite;

    void renderSprite(const glm::vec2 &position, const Sprite &sprite, const U32 depth = 0);

    void renderSprite(const Sprite &sprite, const U32 depth = 0);

    void renderRect(const glm::vec2 &size, const U32 color, const U32 depth);
}