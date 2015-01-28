#pragma once

#include "Renderer.h"
#include "TextureLoader.h"

namespace engine {
    class SDLTextureLoader : public TextureLoader {
    public:

        SDLTextureLoader(Renderer *renderer);

        SDLTextureLoader(const TextureLoader &other) = delete;

        SDLTextureLoader operator=(const TextureLoader &other) = delete;

        std::unique_ptr<engine::Texture> load(std::string filename) override;

    private:
        Renderer *renderer;
    };
}