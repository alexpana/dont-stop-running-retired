#pragma once

#include "Engine.h"
#include "Memory.h"
#include "Renderer.h"
#include "Rect2.h"
#include "Texture.h"
#include "TileMap.h"
#include "Vec2.h"

namespace engine {
    class TileEngine;

    typedef std::shared_ptr<TileEngine> TileEnginePtr;

    class TileEngine {
    public:
        TileEngine(Renderer *renderer, TileMap *tileMap, Texture *tileAtlas) :
                renderer(renderer),
                tileMap(tileMap),
                atlas(tileAtlas) {
        }

        void drawTile(double x, double y, TileType type) {
            const Rect2 tileSourceRect = tileMap->getTileRect(type);

            renderer->drawTexture(atlas, Vec2(x, y), &tileSourceRect);
        }

        void drawTile(Vec2 v, TileType type) {
            drawTile(v.x, v.y, type);
        }

    private:
        Renderer *renderer;

        TileMap *tileMap;

        Texture *atlas;
    };
}