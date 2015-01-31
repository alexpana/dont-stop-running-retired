#pragma once

#include "../game_object.h"
#include "../renderer.h"

namespace dsr {
    class ShapeObject : public GameObject {
    public:
        ShapeObject(const std::string &name, glm::vec2 size, bool solid, bool deadly) :
                GameObject(name, size, solid, deadly) {

        }

        virtual void render() {
            renderSprite(<#(const glm::vec2&)position#>, <#(dsr::Sprite const &)sprite#>, <#(U32 const)depth#>)
        }

        virtual void update(F32 ms) {
            // nothing to do
        }

    private:
        U32 color;
    };
}