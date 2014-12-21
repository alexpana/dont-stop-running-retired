#pragma once

#include <memory>

#include <Texture.h>

struct SDL_Texture;

namespace engine {
    class Vec2;

    class SDLTexture : public Texture {
    public:
        SDLTexture(void *texture);

        // Textures are handles to native resources which are expensive / impossible to copy.
        SDLTexture(const SDLTexture &other) = delete;

        ~SDLTexture();

        void *getNative() const override;

        Vec2 getSize() const override;

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}