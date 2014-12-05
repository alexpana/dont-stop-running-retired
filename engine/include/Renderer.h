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
        Renderer(SDL_Renderer *renderer);

        void setColor(int rgba);

        void setColor(int red, int green, int blue, int alpha);

        void setColor(int red, int green, int blue);

        void drawLine(const Vec2 &from, const Vec2 &to);

        void drawRect(const Rect2 &rect);

        void drawCircle(const Vec2 &origin, double radius);

        void drawTexture(const TexturePtr source, const Vec2 &position, const Rect2 &sourceRect);

        void drawTexture(const TexturePtr source, const Vec2 &position);

        void fillRect(const Rect2 &rect);

        void drawText(const Vec2 &position, const std::string &text);

        void clear();

        void flip();

        static RendererPtr create(SDL_Renderer *renderer);

    private:
        struct Implementation;

        std::shared_ptr<Implementation> impl;
    };
}
#pragma clang diagnostic pop