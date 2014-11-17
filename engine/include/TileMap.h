#pragma once

#include <map>
#include <memory>

#include <SDL.h>

namespace engine {

class Rect2;

enum TileType {
    GRASS_TOP = 0,
    GRASS_FILL,
    GROUND,
    RUNNER,
    TILE_TYPE_COUNT
};

class TileMap;
typedef std::shared_ptr<TileMap> TileMapPtr;

class TileMap {
public:
    TileMap();

    virtual ~TileMap();

    void setTileRect(TileType type, int x, int y, int w, int h);

    const Rect2 TileMap::getTileRect(TileType tileType);

private:
    std::map<TileType, Rect2> m_map;
};


}