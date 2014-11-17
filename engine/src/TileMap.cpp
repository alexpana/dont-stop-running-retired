#include "TileMap.h"

#include "Rect2.h"

namespace engine {

TileMap::TileMap()
{
    setTileRect(GRASS_TOP, 0, 0, 32, 32);
    setTileRect(GRASS_FILL, 0, 0, 32, 32);
    setTileRect(GROUND, 32, 0, 32, 32);
    setTileRect(RUNNER, 64, 0, 32, 64);
}

TileMap::~TileMap()
{
}

void TileMap::setTileRect(TileType type, int x, int y, int w, int h)
{
    m_map[type] = Rect2{x, y, w, h};
}

const Rect2 TileMap::getTileRect(TileType tileType)
{
    return m_map[tileType];
}


}