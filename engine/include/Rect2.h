#pragma once

#include <SDL.h>

#include "Vec2.h"

namespace engine {

class Rect2
{
public:
    Rect2() : x(0), y(0), w(0), h(0) {}

    Rect2(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {}

    Rect2(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    SDL_Rect toSDLRect() const
    {
        SDL_Rect sdlRect = {(int)x, (int)y, (int)w, (int)h};
        return sdlRect;
    }

    bool contains(const Vec2& point) const
    {
        return point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + w;
    }

    bool intersects(const Rect2& rhs) const
    {
        return contains(rhs.topLeft()) || contains(rhs.topRight()) || contains(rhs.bottomLeft()) || contains(rhs.bottomRight());
    }

    Rect2 translated(const Vec2& offset)
    {
        return Rect2(x + offset.x, y + offset.y, w, h);
    }

    Vec2 centerOfMass() const
    {
        return Vec2(x + w / 2, y + h / 2);
    }

    Vec2 topLeft() const { return Vec2(x, y); }
    Vec2 topRight() const { return Vec2(x + w, y); }
    Vec2 bottomLeft() const { return Vec2(x, y + h); }
    Vec2 bottomRight() const { return Vec2(x + w, y + h); }
    
public:
    double x;
    double y;
    double w;
    double h;
};

}