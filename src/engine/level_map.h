#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "game_object.h"

namespace dsr {
    class LevelMap {
    public:
        struct Entity {
            glm::vec2 position;
            glm::vec2 scale;
            F32 rotation;
            GameObject gameObject;
        };

        glm::vec2 playerStartPosition;

        std::vector<Entity> entities;

        LevelMap(LevelMap &&other) = default;
        LevelMap& operator=(LevelMap &&other) = default;
    };
}