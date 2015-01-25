#include "asset_loader.h"

#include "json_reader.h"

using namespace std;
using namespace rapidjson;

static dsr::Log _log{"asset_loader"};

namespace dsr {

    template<typename T>
    bool loadSprite(T &obj, Sprite &sprite) {
        auto nameIt = obj.FindMember("sheet");
        if (nameIt != obj.MemberEnd()) {
            sprite.sheetName = nameIt->value.GetString();
        } else {
            _log.error("Could not load sprite name.");
            return false;
        }

        auto offsetIt = obj.FindMember("offset");
        if (offsetIt != obj.MemberEnd()) {
            sprite.offset.x = offsetIt->value[0].GetDouble();
            sprite.offset.y = offsetIt->value[1].GetDouble();
        } else {
            _log.error("Could not load sprite offset.");
            return false;
        }

        auto sizeIt = obj.FindMember("size");
        if (sizeIt != obj.MemberEnd()) {
            sprite.size.x = sizeIt->value[0].GetDouble();
            sprite.size.y = sizeIt->value[1].GetDouble();
        } else {
            _log.error("Could not load sprite size.");
            return false;
        }

        auto originIt = obj.FindMember("origin");
        if (originIt != obj.MemberEnd()) {
            sprite.origin.x = originIt->value[0].GetDouble();
            sprite.origin.y = originIt->value[1].GetDouble();
        } else {
            _log.warn("Sprite origin not specified. Defaulting to (0, 0).");
            sprite.origin = glm::vec2(0, 0);
        }

        return true;
    }

    template<typename T>
    bool loadObject(T &obj, GameObject &gameObject) {
        auto nameIt = obj.FindMember("name");
        if (nameIt != obj.MemberEnd()) {
            gameObject.name = nameIt->value.GetString();
        } else {
            _log.error("Could not load the game object name.");
            return false;
        }

        _log.info("Loading object ", gameObject.name);

        auto sizeIt = obj.FindMember("size");
        if (sizeIt != obj.MemberEnd()) {
            gameObject.size.x = sizeIt->value[0].GetDouble();
            gameObject.size.y = sizeIt->value[1].GetDouble();
        } else {
            _log.error("Could not load the game object size.");
            return false;
        }

        auto spriteIt = obj.FindMember("sprite");
        if (spriteIt != obj.MemberEnd()) {
            loadSprite(spriteIt->value, gameObject.sprite);
        } else {
            _log.error("Could not load the game object sprite definition.");
            return false;
        }

        return true;
    }

    vector<GameObject> loadObjects(const std::string &jsonFilename) {
        JSONDocument doc(jsonFilename);
        vector<GameObject> gameObjects;

        auto &json = doc.getDocument();

        U32 count = 0;

        _log.info("Loading objects from: ", jsonFilename);
        auto findIt = json.FindMember("objects");
        if (findIt != json.MemberEnd()) {
            count = findIt->value.Size();
            for (U32 i = 0; i < count; ++i) {
                GameObject object;
                bool loadSuccess = loadObject(findIt->value[i], object);
                if (loadSuccess) {
                    gameObjects.push_back(object);
                }
            }
        } else {
            _log.warn("No definition for \"objects\" was found in the script.");
        }

        _log.info("Successfuly loaded ", gameObjects.size(), " out of ", count, " objects.");

        return gameObjects;
    }

    void loadSprites(const std::string &jsonFilename) {
        _log.error(__FUNCTION__, " is not implemented");
    }

    void loadParticleSystems(const std::string &jsonFilename) {
        _log.error(__FUNCTION__, " is not implemented");
    }
}