#pragma once

#include <memory>

#include "Game.h"
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
        TileEngine(RendererPtr renderer, TileMapPtr tileMap, TexturePtr tileAtlas) :
                m_renderer(renderer),
                m_tileMap(tileMap),
                m_atlas(tileAtlas) {
        }

        void drawTile(double x, double y, TileType type) {
            const Rect2 tileSourceRect = m_tileMap->getTileRect(type);

            m_renderer->drawTexture(m_atlas, Vec2(x, y), tileSourceRect);
        }

        void drawTile(Vec2 v, TileType type) {
            drawTile(v.x, v.y, type);
        }

    private:
        RendererPtr m_renderer;

        TileMapPtr m_tileMap;

        TexturePtr m_atlas;
    };
}