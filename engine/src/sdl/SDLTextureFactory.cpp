#include "sdl/SDLTextureFactory.h"

#include "sdl/SDLTexture.h"

namespace engine {

    SDLTextureFactory::SDLTextureFactory(Renderer *renderer) : renderer(renderer), log("SDLTextureFactory") {
    }

    std::unique_ptr<engine::Texture> SDLTextureFactory::loadTextureFromImage(std::string filename) {
        SDL_Surface *rawSurface = IMG_Load(filename.c_str());

        if (rawSurface == nullptr) {
            log.error() << "Could not load surface" << filename << "\n";
            log.error() << "IMG_Load: " << IMG_GetError() << "\n";
            return nullptr;
        }

        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(
                reinterpret_cast<SDL_Renderer *>(renderer->getNativeRenderer()), rawSurface);

        SDLTexture *texture = new SDLTexture{sdlTexture};

        auto result = std::unique_ptr<Texture>(texture);

        return result;
    }
}