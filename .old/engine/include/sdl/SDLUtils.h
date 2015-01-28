#pragma once

#include <SDL.h>

namespace engine {

    class Rect2;

    class Vec2;

    SDL_Rect toSdlRect(const Rect2 &rect);

    SDL_Point toSdlPoint(const Vec2 &vec);
}