#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Log.h"
#include "Rect2.h"
#include "sdl/SDLTexture.h"

static engine::Log _log{"Renderer"};

namespace engine {

    struct Renderer::Implementation {
    public:
        SDL_Renderer *renderer;
        TTF_Font *font;
        Renderer::TextureAnchor textureAnchor = TextureAnchor::TOP_LEFT;

        Implementation(SDL_Renderer *renderer) : renderer(renderer) {
        }

        void drawSurface(SDL_Surface *surface, const Vec2 &position);

        SDL_Point drawOffset(int width, int height) {
            switch (textureAnchor) {
                case TextureAnchor::TOP_LEFT:
                    return SDL_Point{0, 0};
                case TextureAnchor::CENTER:
                    return SDL_Point{-width / 2, -height / 2};
            };
        }
    };

    Renderer::Renderer(SDL_Renderer *renderer) {
        impl = std::unique_ptr<Implementation>(new Implementation{renderer});

        impl->font = TTF_OpenFont("data/minecraftia.ttf", 8);

        SDL_SetRenderDrawBlendMode(impl->renderer, SDL_BLENDMODE_BLEND);
    }

    Renderer::~Renderer() {
    }

    void Renderer::setColor(unsigned int rgba) {
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

    void Renderer::drawTexture(const Texture *source, const Vec2 &position, const Rect2 *sourceRect, const double rotation) {
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        bool hasSourceRect = false;

        if (sourceRect) {
            srcRect = {(int) sourceRect->x, (int) sourceRect->y, (int) sourceRect->w, (int) sourceRect->h};
            hasSourceRect = true;
        }

        if (sourceRect) {
            dstRect = {(int) position.x, (int) position.y, (int) sourceRect->w, (int) sourceRect->h};
        } else {
            dstRect = {(int) position.x, (int) position.y, (int) source->getSize().w, (int) source->getSize().h};
        }

        SDL_Point center = impl->drawOffset((int) source->getSize().w, (int) source->getSize().h);

        dstRect.x += center.x;
        dstRect.y += center.y;

        SDL_RenderCopyEx(impl->renderer, reinterpret_cast<SDL_Texture *>(source->getNative()), hasSourceRect ? &srcRect : (SDL_Rect *) nullptr, &dstRect, rotation, nullptr, SDL_FLIP_NONE);
    }

    void Renderer::drawTexture(const Texture *source, const Vec2 &position, double rotation) {
        drawTexture(source, position, nullptr, rotation);
    }

    void Renderer::flip() {
        SDL_RenderPresent(impl->renderer);
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

    std::unique_ptr<Texture> Renderer::createTexture(const Vec2 &size, int access = SDL_TEXTUREACCESS_TARGET) {

        SDL_Texture *sdlTexture = SDL_CreateTexture(impl->renderer, SDL_PIXELFORMAT_RGBA8888, access, (int) size.w, (int) size.h);

        if (!sdlTexture) {
            _log.error() << "Could not create texture. Reason: " << SDL_GetError() << "\n";
        } else {
            SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
        }

        Texture *texture = new SDLTexture{sdlTexture};

        std::unique_ptr<Texture> result{texture};

        return result;
    }

    void Renderer::setTarget(Texture *texture) {
        int error = SDL_SetRenderTarget(impl->renderer, reinterpret_cast<SDL_Texture *>(texture->getNative()));
        if (error) {
            _log.error() << "Could not set render target. Reason: " << SDL_GetError() << "\n";
        }
    }

    void Renderer::resetTarget() {
        int error = SDL_SetRenderTarget(impl->renderer, nullptr);
        if (error) {
            _log.error() << "Could not reset render target. Reason: " << SDL_GetError() << "\n";
        }
    }

    void Renderer::setAlphaModulation(Texture *texture, int modulation) {
        int error = SDL_SetTextureAlphaMod(reinterpret_cast<SDL_Texture *>(texture->getNative()), (Uint8) (modulation & 0xFF));
        if (error) {
            _log.error() << "Could not set alpha modulation. Reason: " << SDL_GetError() << "\n";
        }
    }

    void Renderer::setTextureAnchor(Renderer::TextureAnchor textureAnchor) {
        impl->textureAnchor = textureAnchor;
    }

    Renderer::TextureAnchor Renderer::getTextureAnchor() {
        return impl->textureAnchor;
    }

    Vec2 Renderer::getViewportSize() {
        SDL_Rect viewportRect;
        SDL_RenderGetViewport(impl->renderer, &viewportRect);
        return engine::Vec2{(double) viewportRect.w, (double) viewportRect.h};
    }

    void *Renderer::getNativeRenderer() {
        return impl->renderer;
    }
}
#pragma clang diagnostic pop