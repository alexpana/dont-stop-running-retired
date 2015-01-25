#include "assets.h"

#include "log.h"

namespace dsr {
    static dsr::Log _log{"Assets"};

    Assets *Assets::uniqueInstance = nullptr;

    Assets &Assets::instance() {
        if (uniqueInstance == nullptr) {
            uniqueInstance = new Assets();
        }

        return *uniqueInstance;
    }

    Assets::Assets() {
    }

    void Assets::registerGameObject(const std::string &name, const GameObject &gameObject) {
        _log.info("Registered game object ", name);
        gameObjectMap[name] = gameObject;
    }

    void Assets::registerSprite(const std::string &name, const Sprite &sprite) {
        _log.info("Registered sprite ", name);
        spriteMap[name] = sprite;
    }

    void Assets::registerTexture(const std::string &name, Texture &texture) {
        _log.info("Registered texture ", name);
        textureMap[name] = std::move(texture);
    }

    void Assets::registerShader(const std::string &name, Shader &shader) {
        _log.info("Registered shader ", name);
        shaderMap[name] = std::move(shader);
    }

    GameObject *Assets::findGameObject(const std::string &name) {
        return findResource(name, gameObjectMap);
    }

    Sprite *Assets::findSprite(const std::string &name) {
        return findResource(name, spriteMap);
    }

    Texture *Assets::findTexture(const std::string &name) {
        return findResource(name, textureMap);
    }

    Shader *Assets::findShader(const std::string &name) {
        return findResource(name, shaderMap);
    }
}
