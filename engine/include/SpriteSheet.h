#pragma once

#include <string>
#include <unordered_map>

namespace engine {
    class Texture;

    class Sprite;

    class SpriteSheet {
    public:
        Texture *getTexture() const {
            return texture.get();
        }

        Sprite *getSprite(const std::string sprite) const {
            auto findIt = sprites.find(sprite);
            if (findIt != sprites.end()) {
                return findIt->second.get();
            }

            return nullptr;
        }

        void setTexture(std::unique_ptr<Texture>&& texture) {
            this->texture = std::move(texture);
        }

        bool addSprite(const std::string& name, std::unique_ptr<Sprite>&& sprite) {
            sprites[name] = std::move(sprite);
            return true;
        }

    private:
        std::unique_ptr<Texture> texture;
        std::unordered_map<std::string, std::unique_ptr<Sprite>> sprites;
    };
}