#pragma once

#include <vector>

#include <Memory.h>

namespace engine {
    class Texture;

    class Renderer;
}

class BackgroundRenderer {
public:
    BackgroundRenderer();

    ~BackgroundRenderer();

    void addTexture(engine::Texture *texture, double speed);

    void removeTexture(engine::Texture *texture);

    void render(engine::Renderer *renderer, double offset);

private:
    struct Implementation;
    std::unique_ptr<Implementation> impl;
};