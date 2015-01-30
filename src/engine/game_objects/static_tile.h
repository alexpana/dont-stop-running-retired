#pragma once

#include "../game_object.h"

namespace dsr {
    class StaticTile : public GameObject {
    public:
        StaticTile(Sprite *sprite, std::string name, glm::vec2 size, bool isSolid = true, bool isDeadly = false) :
                GameObject(name, size, isSolid, isDeadly),
                sprite(sprite) {
        }

        void render() override;

        void update(F32 ms) override {
            // nothing to do
        }

    private:
        Sprite *sprite;
    };
}