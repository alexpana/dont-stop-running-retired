#pragma once

#include <memory>

#include <SDL_image.h>

#include "Texture.h"

namespace engine {
    struct TextureFactory {
        virtual std::unique_ptr<engine::Texture> loadTextureFromImage(std::string filename) = 0;

        virtual ~TextureFactory() {
        }
    };
}