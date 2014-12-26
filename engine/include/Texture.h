#pragma once

namespace engine {
    class Vec2;

    struct Texture {
        virtual void *getNative() const = 0;

        virtual Vec2 getSize() const = 0;

        virtual ~Texture() {
        }
    };
}