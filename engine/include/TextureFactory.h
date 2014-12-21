#pragma once

#include <memory>

#include <SDL_image.h>

#include "Texture.h"

namespace engine {
    class TextureFactory;

    class TextureFactory {

    public:
        TextureFactory(SDL_Renderer *renderer, const SDL_PixelFormat &optimizedPixelFormat);

        TextureFactory(const TextureFactory &other) = delete;

        TextureFactory operator=(const TextureFactory &other) = delete;

        std::unique_ptr<engine::Texture> loadImage(std::string filename);

    private:
        const SDL_PixelFormat m_optimizedPixelFormat;

        SDL_Renderer *m_nativeRenderer;
    };
}