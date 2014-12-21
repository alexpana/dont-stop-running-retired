#pragma once

#include "Log.h"
#include "Renderer.h"
#include "TextureFactory.h"

namespace engine {
    class SDLTextureFactory : public TextureFactory {
    public:

        SDLTextureFactory(Renderer *renderer);

        SDLTextureFactory(const TextureFactory &other) = delete;

        SDLTextureFactory operator=(const TextureFactory &other) = delete;

        std::unique_ptr<engine::Texture> loadTextureFromImage(std::string filename) override;

    private:
        Renderer *renderer;
        Log log;
    };
}