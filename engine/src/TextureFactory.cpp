#include "TextureFactory.h"

#include <stdio.h>

namespace engine {

    TextureFactory::TextureFactory(SDL_Renderer *renderer, const SDL_PixelFormat &optimizedPixelFormat) :
            m_optimizedPixelFormat(optimizedPixelFormat),
            m_nativeRenderer(renderer),
            m_dummyImage(nullptr) {
    }

    TexturePtr TextureFactory::loadImage(std::string filename) {
        SDL_Surface *rawSurface = IMG_Load(filename.c_str());

        if (rawSurface == nullptr) {
            printf("%s: %s\n", "Could not load surface", filename.c_str());
            printf("IMG_Load: %s\n", IMG_GetError());
            return m_dummyImage;
        }

        return Texture::create(SDL_CreateTextureFromSurface(m_nativeRenderer, rawSurface));
    }

    TextureFactoryPtr TextureFactory::create(SDL_Renderer *renderer, const SDL_PixelFormat &optimizedPixelFormat) {
        return std::make_shared<TextureFactory>(renderer, optimizedPixelFormat);
    }
}