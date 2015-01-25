#pragma once

#include <unordered_map>
#include <vector>

#include "game_object.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

namespace dsr {

    /**
    * Global repository for all game assets.
    */
    class Assets {
    public:
        Assets(const Assets &other) = delete;

        void registerGameObject(const std::string &name, const GameObject &gameObject);

        void registerSprite(const std::string &name, const Sprite &sprite);

        void registerTexture(const std::string &name, Texture &texture);

        void registerShader(const std::string &name, Shader &shader);

        GameObject *findGameObject(const std::string &name);

        Sprite *findSprite(const std::string &name);

        Texture *findTexture(const std::string &name);

        Shader *findShader(const std::string &name);

        static Assets &instance();

    private:
        Assets();

        static Assets *uniqueInstance;

        template<typename M>
        typename M::mapped_type *findResource(const std::string &name, M &map) {
            auto findIt = map.find(name);

            if (findIt != map.end()) {
                return &findIt->second;
            }

            return nullptr;
        }

        std::unordered_map<std::string, GameObject> gameObjectMap;
        std::unordered_map<std::string, Sprite> spriteMap;
        std::unordered_map<std::string, Texture> textureMap;
        std::unordered_map<std::string, Shader> shaderMap;
    };
}