#include "assets.h"

#include "log.h"

namespace dsr {
    static dsr::Log _log{"Assets"};

    UPtr<Assets> Assets::uniqueInstance = nullptr;

    Assets &Assets::instance() {
        if (uniqueInstance == nullptr) {
            uniqueInstance = UPtr<Assets>(new Assets());
        }

        return *uniqueInstance.get();
    }

    Assets::Assets() {
        _log.info("Creating a new assets instance.");
    }

    Assets::~Assets() {
        std::cout << "Destroying assets instance." << std::endl;
    }

    template<typename M>
    typename M::mapped_type::element_type *findResource(const std::string &name, M &map) {
        auto findIt = map.find(name);

        if (findIt != map.end()) {
            return findIt->second.get();
        }

        return nullptr;
    }

    void Assets::registerGameObject(const std::string &name, std::unique_ptr<StaticTile> &&gameObject) {
        _log.info("Registered game object ", name);
        gameObjectMap[name] = std::move(gameObject);
    }

    void Assets::registerSprite(const std::string &name, std::unique_ptr<Sprite> &&sprite) {
        _log.info("Registered sprite ", name);
        spriteMap[name] = std::move(sprite);
    }

    void Assets::registerTexture(const std::string &name, std::unique_ptr<Texture> &&texture) {
        _log.info("Registered texture ", name);
        textureMap[name] = std::move(texture);
    }

    void Assets::registerShader(const std::string &name, std::unique_ptr<Shader> &&shader) {
        _log.info("Registered shader ", name);
        shaderMap[name] = std::move(shader);
    }

    StaticTile *Assets::findGameObject(const std::string &name) {
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

    void Assets::destroyInstance() {
        if (uniqueInstance) {
            uniqueInstance.reset(nullptr);
        }
    }
}
