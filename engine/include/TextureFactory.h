#pragma once

#include <memory>

#include <SDL_image.h>

#include "Texture.h"

namespace engine {
    class TextureFactory {

    public:
        virtual std::unique_ptr<engine::Texture> loadTextureFromImage(std::string filename) = 0;
    };
}