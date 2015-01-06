#include "sdl/SDLUtils.h"

#include "Rect2.h"

namespace engine {

    SDL_Rect toSdlRect(const Rect2 &rect) {
        SDL_Rect sdlRect = {(int) rect.x, (int) rect.y, (int) rect.w, (int) rect.h};
        return sdlRect;
    }

    SDL_Point toSdlPoint(const Vec2 &vec) {
        SDL_Point sdlPoint = {(int) vec.x, (int) vec.y};
        return sdlPoint;
    }

}