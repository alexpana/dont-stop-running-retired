#include "graphics/Renderer.h"

namespace engine {
    namespace graphics {

        void Renderer::setColor(int rgba) {
            setColor((rgba >> 24) & 0xff, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff);
        }

        void Renderer::setColor(int red, int green, int blue) {
            setColor(red, green, blue, 255);
        }

        void Renderer::setColor(int red, int green, int blue, int alpha) {
            SDL_SetRenderDrawColor(m_renderer,
                    static_cast<Uint8>(0xFF & red),
                    static_cast<Uint8>(0xFF & green),
                    static_cast<Uint8>(0xFF & blue),
                    static_cast<Uint8>(0xFF & alpha));
        }

        void Renderer::drawLine(const Vec2 &from, const Vec2 &to) {
            SDL_RenderDrawLine(m_renderer, (int) from.x, (int) from.y, (int) to.x, (int) to.y);
        }

        void Renderer::drawRect(const Rect2 &rect) {
            auto sdlRect = rect.toSDLRect();
            SDL_RenderDrawRect(m_renderer, &sdlRect);
        }

        void Renderer::drawCircle(const Vec2 &origin, double radius) {
            SDL_Rect rect = {(int) (origin.x - radius), (int) (origin.y - radius), (int) radius * 2, (int) radius * 2};
            SDL_RenderDrawRect(m_renderer, &rect);
        }

        void Renderer::clear() {
            SDL_RenderClear(m_renderer);
        }

        void Renderer::fillRect(const Rect2 &rect) {
            auto sdlRect = rect.toSDLRect();
            SDL_RenderFillRect(m_renderer, &sdlRect);
        }

        void Renderer::drawTexture(const TexturePtr source, const Vec2 &position, const Rect2 &sourceRect) {
            SDL_Rect srcRect = {(int) sourceRect.x, (int) sourceRect.y, (int) sourceRect.w, (int) sourceRect.h};
            SDL_Rect dstRect = {(int) position.x, (int) position.y, (int) sourceRect.w, (int) sourceRect.h};
            SDL_RenderCopy(m_renderer, source->getNative(), &srcRect, &dstRect);
        }

        void Renderer::drawTexture(const TexturePtr source, const Vec2 &position) {
            SDL_Rect dstRect = {(int) position.x, (int) position.y, source->getWidth(), source->getHeight()};
            SDL_RenderCopy(m_renderer, source->getNative(), nullptr, &dstRect);
        }

        void Renderer::flip() {
            SDL_RenderPresent(m_renderer);
        }

        RendererPtr Renderer::create(SDL_Renderer *renderer) {
            return std::make_shared<Renderer>(renderer);
        }
    }
}