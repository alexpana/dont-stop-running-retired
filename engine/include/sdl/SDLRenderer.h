#pragma once

#include "Renderer.h"

namespace engine {
    class SDLRenderer : public Renderer {
    public:
        SDLRenderer(void *nativeRenderer);

        ~SDLRenderer();

        void setTextureAnchor(TextureAnchor textureAnchor) override;

        TextureAnchor getTextureAnchor() override;

        void setColor(unsigned int rgba) override;

        void setColor(int red, int green, int blue, int alpha) override;

        void setColor(int red, int green, int blue) override;

        void drawLine(const Vec2 &from, const Vec2 &to) override;

        void drawRect(const Rect2 &rect) override;

        void drawCircle(const Vec2 &origin, double radius) override;

        void drawTexture(const Texture *source, const Vec2 &position, const Rect2 *sourceRect, const double rotation = 0) override;

        void drawTexture(const Texture *source, const Vec2 &position, double rotation = 0) override;

        void fillRect(const Rect2 &rect) override;

        void drawText(const Vec2 &position, const std::string &text) override;

        Vec2 getViewportSize() override;

        void clear() override;

        void flip() override;

        std::unique_ptr<Texture> createTexture(const Vec2 &size, int access) override;

        void setTarget(Texture *texture) override;

        void resetTarget() override;

        void setAlphaModulation(Texture *texture, int modulation) override;

        void *getNativeRenderer() override;

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}