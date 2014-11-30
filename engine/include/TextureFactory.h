#pragma once

#include <memory>

#include <SDL_image.h>

#include "Texture.h"

namespace engine {
    class TextureFactory;

    typedef std::shared_ptr<TextureFactory> TextureFactoryPtr;

    class TextureFactory {

    public:
        TextureFactory(SDL_Renderer *renderer, const SDL_PixelFormat &optimizedPixelFormat);

        TextureFactory(const TextureFactory &other) = delete;

        TextureFactory operator=(const TextureFactory &other) = delete;

        TexturePtr loadImage(std::string filename);

        static TextureFactoryPtr create(SDL_Renderer *renderer, const SDL_PixelFormat &optimizedPixelFormat);

    private:
        const SDL_PixelFormat m_optimizedPixelFormat;

        SDL_Renderer *m_nativeRenderer;

        const TexturePtr m_dummyImage;
    };
}