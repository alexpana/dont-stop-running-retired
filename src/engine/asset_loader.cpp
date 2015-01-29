#include "asset_loader.h"

#include <memory>

#include <bx/bx.h>
#include <bx/readerwriter.h>

#include "json.h"

using namespace std;
using namespace rapidjson;

static dsr::Log _log{"AssetLoader"};

namespace dsr {

    static std::unique_ptr<bx::FileReaderI> sFileReaderHandle = std::unique_ptr<bx::FileReaderI>(new bx::CrtFileReader);

    static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const std::string &_filePath) {
        if (!bx::open(_reader, _filePath.c_str())) {
            U32 size = (U32) bx::getSize(_reader);
            const bgfx::Memory *mem = bgfx::alloc(size + 1);
            bx::read(_reader, mem->data, size);
            bx::close(_reader);
            mem->data[mem->size - 1] = '\0';
            return mem;
        }

        return nullptr;
    }

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

    bool loadTexture(const std::string &filename, Texture &texture) {
        const bgfx::Memory *mem = loadMem(sFileReaderHandle.get(), filename.c_str());

        bgfx::TextureInfo info;

        auto handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE, 0, &info);

        if (bgfx::isValid(handle)) {
            texture = std::move(Texture(handle, info));
            return true;
        }

        return false;
    }

    static bgfx::ShaderHandle loadShader(const std::string &filePath) {
        return bgfx::createShader(loadMem(sFileReaderHandle.get(), filePath));
    }

    static bgfx::ProgramHandle loadProgram(const std::string &shaderName) {
        bgfx::ShaderHandle vsh = loadShader(shaderName + ".vert.bin");
        bgfx::ShaderHandle fsh = loadShader(shaderName + ".frag.bin");

        auto handle = bgfx::createProgram(vsh, fsh, true);

        return handle;
    }

    bool loadShader(const std::string &filename, Shader &shader) {
        bgfx::ProgramHandle handle = loadProgram(filename);
        if (bgfx::isValid(handle)) {
            _log.info("Successfuly loaded shader ", filename);
            shader = std::move(Shader(handle));
            return true;
        }

        _log.warn("Could not load shader ", filename);

        return false;
    }

    bool loadMapLevel(const std::string &filename, LevelMap &levelMap) {
        _log.error(__FUNCTION__, " is not implemented");
        return false;
    }

    void *loadRawFile(const std::string &filename, U32 *size) {
        if (0 == bx::open(sFileReaderHandle.get(), filename.c_str())) {
            U32 _size = (uint32_t) bx::getSize(sFileReaderHandle.get());
            void *data = malloc(_size);
            bx::read(sFileReaderHandle.get(), data, _size);
            bx::close(sFileReaderHandle.get());
            if (size != nullptr) {
                *size = _size;
            }
            return data;
        }
        if (size != nullptr) {
            *size = 0;
        }
        return nullptr;
    }
}