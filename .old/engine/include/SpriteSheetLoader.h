#pragma once

#include <memory>
#include "Rect2.h"
#include "TextureLoader.h"

namespace engine {
    class SpriteSheet;

    class TextureLoader;

    class SpriteSheetLoader {
    public:
        SpriteSheetLoader(TextureLoader *textureLoader) : textureLoader(textureLoader) {
        }

        std::unique_ptr<SpriteSheet> load(const std::string descriptionFile);

    private:
        TextureLoader *textureLoader;
    };
}