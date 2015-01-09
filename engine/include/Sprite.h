#pragma once

#include "Rect2.h"
#include "Texture.h"
#include "Vec2.h"

namespace engine {
    class Sprite {
    public:
        Sprite(Rect2 bounds, Vec2 origin, Texture *texture) :
                bounds(bounds), origin(origin), texture(texture) {
        }

        Rect2 getBounds() const {
            return bounds;
        }

        Vec2 getOrigin() const {
            return origin;
        }

        Texture *getTexture() const {
            return texture;
        }

    private:
        Rect2 bounds;
        Vec2 origin;
        Texture *texture;
    };
}