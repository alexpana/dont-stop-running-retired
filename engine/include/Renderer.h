#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <memory>

class SDL_Renderer;

namespace engine {

    class Vec2;

    class Rect2;

    class Texture;

    typedef std::shared_ptr<Texture> TexturePtr;

    class Renderer;

    typedef std::shared_ptr<Renderer> RendererPtr;

    class Renderer {
    public:

        enum class TextureAnchor {
            TOP_LEFT,
            CENTER
        };

        Renderer(SDL_Renderer *renderer);

        ~Renderer();

        Renderer(const Renderer &other) = delete;

        void setTextureAnchor(TextureAnchor textureAnchor);

        TextureAnchor getTextureAnchor();

        void setColor(unsigned int rgba);

        void setColor(int red, int green, int blue, int alpha);

        void setColor(int red, int green, int blue);

        void drawLine(const Vec2 &from, const Vec2 &to);

        void drawRect(const Rect2 &rect);

        void drawCircle(const Vec2 &origin, double radius);

        void drawTexture(const Texture *source, const Vec2 &position, const Rect2 *sourceRect, const double rotation = 0);

        void drawTexture(const Texture *source, const Vec2 &position, double rotation = 0);

        void fillRect(const Rect2 &rect);

        void drawText(const Vec2 &position, const std::string &text);

        Vec2 getViewportSize();

        void clear();

        void flip();

        std::unique_ptr<Texture> createTexture(int w, int h, int access);

        void setTarget(Texture *texture);

        void resetTarget();

        void setAlphaModulation(Texture *texture, int modulation);

        static RendererPtr create(SDL_Renderer *renderer);

    private:
        struct Implementation;

        std::unique_ptr<Implementation> impl;
    };
}
#pragma clang diagnostic pop