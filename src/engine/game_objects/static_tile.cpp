#include "static_tile.h"

#include "../renderer.h"

namespace dsr {
    void StaticTile::render() {
        renderSprite(*sprite, 0);
    }
}