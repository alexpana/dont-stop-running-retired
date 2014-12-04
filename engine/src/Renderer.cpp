#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "graphics/Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Texture.h"

#include "Rect2.h"

namespace engine {
    namespace graphics {

        struct Renderer::Implementation {
        public:
            SDL_Renderer *renderer;
            TTF_Font *font;

            Implementation(SDL_Renderer *renderer) : renderer(renderer) {
            }

            void drawSurface(SDL_Surface *surface, const Vec2& position);
        };

        Renderer::Renderer(SDL_Renderer *renderer) {
            impl = new Implementation(renderer);
            impl->font = TTF_OpenFont("data/minecraftia.ttf", 8);
        }

        Renderer::~Renderer() {
            delete impl;
        }

        void Renderer::setColor(int rgba) {
            setColor((rgba >> 24) & 0xff, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff);
        }

        void Renderer::setColor(int red, int green, int blue) {
            setColor(red, green, blue, 255);
        }

        void Renderer::setColor(int red, int green, int blue, int alpha) {
            SDL_SetRenderDrawColor(impl->renderer,
                    static_cast<Uint8>(0xFF & red),
                    static_cast<Uint8>(0xFF & green),
                    static_cast<Uint8>(0xFF & blue),
                    static_cast<Uint8>(0xFF & alpha));
        }

        void Renderer::drawLine(const Vec2 &from, const Vec2 &to) {
            SDL_RenderDrawLine(impl->renderer, (int) from.x, (int) from.y, (int) to.x, (int) to.y);
        }

        void Renderer::drawRect(const Rect2 &rect) {
            auto sdlRect = rect.toSDLRect();
            SDL_RenderDrawRect(impl->renderer, &sdlRect);
        }

        void Renderer::drawCircle(const Vec2 &origin, double radius) {
            SDL_Rect rect = {(int) (origin.x - radius), (int) (origin.y - radius), (int) radius * 2, (int) radius * 2};
            SDL_RenderDrawRect(impl->renderer, &rect);
        }

        void Renderer::clear() {
            SDL_RenderClear(impl->renderer);
        }

        void Renderer::fillRect(const Rect2 &rect) {
            auto sdlRect = rect.toSDLRect();
            SDL_RenderFillRect(impl->renderer, &sdlRect);
        }

        void Renderer::drawTexture(const TexturePtr source, const Vec2 &position, const Rect2 &sourceRect) {
            SDL_Rect srcRect = {(int) sourceRect.x, (int) sourceRect.y, (int) sourceRect.w, (int) sourceRect.h};
            SDL_Rect dstRect = {(int) position.x, (int) position.y, (int) sourceRect.w, (int) sourceRect.h};
            SDL_RenderCopy(impl->renderer, source->getNative(), &srcRect, &dstRect);
        }

        void Renderer::drawTexture(const TexturePtr source, const Vec2 &position) {
            SDL_Rect dstRect = {(int) position.x, (int) position.y, source->getWidth(), source->getHeight()};
            SDL_RenderCopy(impl->renderer, source->getNative(), nullptr, &dstRect);
        }

        void Renderer::flip() {
            SDL_RenderPresent(impl->renderer);
        }

        RendererPtr Renderer::create(SDL_Renderer *renderer) {
            return std::make_shared<Renderer>(renderer);
        }

        void Renderer::drawText(const Vec2 &position, const std::string &text) {
            SDL_Color color{0x00, 0x00, 0x00, 0xFF};
            SDL_Surface *textSurface = TTF_RenderText_Solid(impl->font, text.c_str(), color);
            impl->drawSurface(textSurface, position);
            SDL_FreeSurface(textSurface);
        }

        void Renderer::Implementation::drawSurface(SDL_Surface *surface, const Vec2 &position) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            int w, h;
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            SDL_Rect dstRect = {(int) position.x, (int) position.y, w, h};
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
}
#pragma clang diagnostic pop