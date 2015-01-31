#pragma once

#include <string>
#include <glm/glm.hpp>

#include "sprite.h"
#include "types.h"

namespace dsr {
    struct GameObject {

        GameObject(std::string &name, glm::vec2 size, bool solid, bool deadly) :
                name(name), size(size), solid(solid), deadly(deadly) {
        }

        virtual std::string getName() {
            return name;
        }

        virtual bool isSolid() {
            return solid;
        };

        virtual bool isDeadly() {
            return deadly;
        }

        virtual glm::vec2 getSize() {
            return size;
        }

        virtual void render() = 0;

        virtual void update(F32 ms) = 0;

        virtual ~GameObject() = default;

    private:
        bool solid;
        bool deadly;
        std::string name;
        glm::vec2 size;
    };
}