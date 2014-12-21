#include "sdl/SDLTexture.h"

#include <SDL_render.h>

#include "Log.h"
#include "Vec2.h"

namespace engine {

    struct SDLTexture::Implementation {
        SDL_Texture *nativeTexture = nullptr;
        Vec2 size = Vec2{0, 0};
    };

    SDLTexture::SDLTexture(void *texture) {
        impl = std::unique_ptr<Implementation>(new Implementation);
        impl->nativeTexture = reinterpret_cast<SDL_Texture *>(texture);

        int w, h;
        SDL_QueryTexture(impl->nativeTexture, nullptr, nullptr, &w, &h);
        impl->size.w = w;
        impl->size.h = h;
    }

    SDLTexture::~SDLTexture() {
        if (impl->nativeTexture != nullptr) {
            SDL_DestroyTexture(impl->nativeTexture);
        }
    }

    void *SDLTexture::getNative() const {
        return impl->nativeTexture;
    }

    Vec2 SDLTexture::getSize() const {
        return impl->size;
    }
}
