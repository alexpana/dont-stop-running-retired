#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <memory>

class SDL_Renderer;

namespace engine {

    class Vec2;

    class Rect2;

    class Texture;

    struct Renderer {
        enum class TextureAnchor {
            TOP_LEFT,
            CENTER
        };

        virtual void setTextureAnchor(TextureAnchor textureAnchor) = 0;

        virtual TextureAnchor getTextureAnchor() = 0;

        virtual void setColor(unsigned int rgba) = 0;

        virtual void setColor(int red, int green, int blue, int alpha) = 0;

        virtual void setColor(int red, int green, int blue) = 0;

        virtual void drawLine(const Vec2 &from, const Vec2 &to) = 0;

        virtual void drawRect(const Rect2 &rect) = 0;

        virtual void drawCircle(const Vec2 &origin, double radius) = 0;

        virtual void drawTexture(const Texture *source, const Vec2 &position, const Rect2 *sourceRect, const double rotation = 0) = 0;

        virtual void drawTexture(const Texture *source, const Vec2 &position, double rotation = 0) = 0;

        virtual void fillRect(const Rect2 &rect) = 0;

        virtual void drawText(const Vec2 &position, const std::string &text) = 0;

        virtual Vec2 getViewportSize() = 0;

        virtual void clear() = 0;

        virtual void flip() = 0;

        virtual std::unique_ptr<Texture> createTexture(const Vec2 &size, int access) = 0;

        virtual void setTarget(Texture *texture) = 0;

        virtual void resetTarget() = 0;

        virtual void setAlphaModulation(Texture *texture, int modulation) = 0;

        virtual void *getNativeRenderer() = 0;

        virtual ~Renderer() {
        };
    };
}
#pragma clang diagnostic pop