#pragma once

#include <memory>

#include <SDL.h>

#include "Texture.h"
#include "Rect2.h"
#include "Vec2.h"

namespace engine {
    namespace graphics {
        class Renderer;

        typedef std::shared_ptr<Renderer> RendererPtr;

        class Renderer {
        public:
            Renderer(SDL_Renderer *renderer) : m_renderer(renderer) {
            }

            ~Renderer() {
            }

            void setColor(int rgba);

            void setColor(int red, int green, int blue, int alpha);

            void setColor(int red, int green, int blue);

            void drawLine(const Vec2 &from, const Vec2 &to);

            void drawRect(const Rect2 &rect);

            void drawCircle(const Vec2 &origin, double radius);

            void drawTexture(const TexturePtr source, const Vec2 &position, const Rect2 &sourceRect);

            void drawTexture(const TexturePtr source, const Vec2 &position);

            void fillRect(const Rect2 &rect);

            void clear();

            void flip();

            static RendererPtr create(SDL_Renderer *renderer);

        private:
            SDL_Renderer *m_renderer;
        };

    }
}