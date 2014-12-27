#pragma once

#include <SDL_image.h>

#include "Memory.h"
#include "Texture.h"

namespace engine {
    struct TextureFactory {
        virtual std::unique_ptr<engine::Texture> loadTextureFromImage(std::string filename) = 0;

        virtual ~TextureFactory() {
        }
    };
}