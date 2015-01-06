#pragma once

#include <SDL_image.h>

#include "Memory.h"
#include "Texture.h"

namespace engine {
    struct TextureLoader {
        virtual std::unique_ptr<engine::Texture> load(std::string filename) = 0;

        virtual ~TextureLoader() {
        }
    };
}