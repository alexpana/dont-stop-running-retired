#pragma once

#include <vector>

#include <SDL.h>

#include "Memory.h"

namespace engine {

    class Rect2;

    enum TileType {
        GRASS_TOP,
        GRASS_FILL,
        GROUND,
        RUNNER,
        TILE_TYPE_COUNT
    };

    class TileMap {
    public:
        TileMap();

        virtual ~TileMap();

        void setTileRect(TileType type, int x, int y, int w, int h);

        const Rect2 getTileRect(TileType tileType);

    private:
        std::vector<Rect2> tiles;
    };
}