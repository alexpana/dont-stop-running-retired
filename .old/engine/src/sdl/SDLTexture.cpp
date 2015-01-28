#include "sdl/SDLTexture.h"

#include <SDL_render.h>

#include "Log.h"
#include "Rect2.h"

namespace engine {

    struct SDLTexture::Implementation {
        SDL_Texture *nativeTexture = nullptr;
        Rect2 bounds;
    };

    SDLTexture::SDLTexture(void *texture) {
        impl = std::make_unique<Implementation>();
        impl->nativeTexture = reinterpret_cast<SDL_Texture *>(texture);

        int w, h;
        SDL_QueryTexture(impl->nativeTexture, nullptr, nullptr, &w, &h);
        impl->bounds = Rect2{0, 0, w, h};
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
        return impl->bounds.size();
    }

    Rect2 SDLTexture::getBounds() const {
        return impl->bounds;
    }
}
