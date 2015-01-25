#pragma once

#include <glm/vec2.hpp>

namespace dsr {
    class Sprite;

    void renderSprite(const glm::vec2& position, const Sprite& sprite);
}