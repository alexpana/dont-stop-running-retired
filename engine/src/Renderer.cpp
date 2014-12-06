#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Log.h"
#include "Rect2.h"
#include "Texture.h"

namespace engine {

    struct Renderer::Implementation {
    public:
        SDL_Renderer *renderer;
        TTF_Font *font;
        Log logger;

        Implementation(SDL_Renderer *renderer) : renderer(renderer), logger("Renderer") {
        }

        void drawSurface(SDL_Surface *surface, const Vec2 &position);
    };

    Renderer::Renderer(SDL_Renderer *renderer) {
        impl = std::unique_ptr<Implementation>(new Implementation{renderer});

        impl->font = TTF_OpenFont("data/minecraftia.ttf", 8);

        SDL_SetRenderDrawBlendMode(impl->renderer, SDL_BLENDMODE_BLEND);
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

    void Renderer::drawTexture(const Texture *source, const Vec2 &position, const Rect2 &sourceRect) {
        SDL_Rect srcRect = {(int) sourceRect.x, (int) sourceRect.y, (int) sourceRect.w, (int) sourceRect.h};
        SDL_Rect dstRect = {(int) position.x, (int) position.y, (int) sourceRect.w, (int) sourceRect.h};
        SDL_RenderCopy(impl->renderer, source->getNative(), &srcRect, &dstRect);
    }

    void Renderer::drawTexture(const Texture *source, const Vec2 &position) {
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

    std::unique_ptr<Texture> Renderer::createTexture(int w, int h, int access = SDL_TEXTUREACCESS_TARGET) {

        SDL_Texture *newTexture = SDL_CreateTexture(impl->renderer, SDL_PIXELFORMAT_RGBA8888, access, w, h);

        if (!newTexture) {
            impl->logger.error() << "Could not create texture. Reason: " << SDL_GetError() << "\n";
        } else {
            SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
        }

        return std::unique_ptr<Texture>(new engine::Texture{newTexture});
    };

    void Renderer::setTarget(Texture *texture) {
        int error = SDL_SetRenderTarget(impl->renderer, texture->getNative());
        if (error) {
            impl->logger.error() << "Could not set render target. Reason: " << SDL_GetError() << "\n";
        }
    }

    void Renderer::resetTarget() {
        int error = SDL_SetRenderTarget(impl->renderer, nullptr);
        if (error) {
            impl->logger.error() << "Could not reset render target. Reason: " << SDL_GetError() << "\n";
        }
    }

    void Renderer::setAlphaModulation(Texture *texture, int modulation) {
        int error = SDL_SetTextureAlphaMod(texture->getNative(), (Uint8) (modulation & 0xFF));
        if (error) {
            impl->logger.error() << "Could not set alpha modulation. Reason: " << SDL_GetError() << "\n";
        }
    }
}
#pragma clang diagnostic pop