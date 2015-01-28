#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "game_object.h"

namespace dsr {
    class LevelMap {
    public:
        struct Entity {
            glm::vec2 position;
            GameObject gameObject;
        };

        glm::vec2 playerStartPosition;

        std::vector<Entity> entities;
    };
}