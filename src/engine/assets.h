#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <ostream>
#include <iostream>

#include "game_object.h"
#include "game_objects/static_tile.h"
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

        void registerGameObject(const std::string &name, std::unique_ptr<StaticTile> &&gameObject);

        void registerSprite(const std::string &name, std::unique_ptr<Sprite> &&sprite);

        void registerTexture(const std::string &name, std::unique_ptr<Texture> &&texture);

        void registerShader(const std::string &name, std::unique_ptr<Shader> &&shader);

        StaticTile *findGameObject(const std::string &name);

        Sprite *findSprite(const std::string &name);

        Texture *findTexture(const std::string &name);

        Shader *findShader(const std::string &name);

        static Assets &instance();

        static void destroyInstance();

        ~Assets();

    private:
        Assets();

        static UPtr<Assets> uniqueInstance;

        std::unordered_map<std::string, std::unique_ptr<StaticTile>> gameObjectMap;
        std::unordered_map<std::string, std::unique_ptr<Sprite>> spriteMap;
        std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    };
}