#include "BackgroundRenderer.h"

#include <algorithm>

#include <Rect2.h>
#include <Renderer.h>
#include <Texture.h>

using namespace engine;

struct BackgroundTexture {
    Texture *texture;
    double speed;
};

struct BackgroundRenderer::Implementation {
    std::vector<BackgroundTexture> textures;

    void drawTilingTexture(Renderer *renderer, Texture *texture, double offset);
};

void BackgroundRenderer::Implementation::drawTilingTexture(Renderer *renderer, Texture *texture, double offset) {
    int roundedX = (int) offset % texture->getWidth();
    Vec2 viewport = renderer->getViewportSize();

    if (roundedX > 0) {
        roundedX -= texture->getWidth();
    }

    Rect2 src{std::abs(roundedX), 0,
            std::min((int) viewport.x, texture->getWidth() + roundedX),
            texture->getHeight()};
    Vec2 dst{0, 0};

    while (dst.x < viewport.x) {
        renderer->drawTexture(texture, dst, &src, 0);
        dst.x += src.w;

        src.x = (int) (src.x + src.w) % texture->getWidth();
        src.w = std::min((int) (viewport.x - dst.x), texture->getWidth());
    }
}

void BackgroundRenderer::addTexture(Texture *texture, double speed) {
    impl->textures.push_back(BackgroundTexture{texture, speed});
}

void BackgroundRenderer::removeTexture(Texture *texture) {
    impl->textures.erase(
            std::remove_if(impl->textures.begin(), impl->textures.end(), [texture](const BackgroundTexture &arg) {
                return arg.texture == texture;
            }),
            impl->textures.end());
}

void BackgroundRenderer::render(Renderer *renderer, double offset) {
    for (auto &backgroundTexture : impl->textures) {
        impl->drawTilingTexture(renderer, backgroundTexture.texture, offset * backgroundTexture.speed);
    }
}

BackgroundRenderer::BackgroundRenderer() {
    impl = std::unique_ptr<Implementation>(new Implementation);
}

BackgroundRenderer::~BackgroundRenderer() {
}
