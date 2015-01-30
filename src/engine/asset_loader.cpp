#include "asset_loader.h"

#include <bx/bx.h>
#include <bx/readerwriter.h>

#include "json.h"
#include "assets.h"

using namespace std;
using namespace rapidjson;

static dsr::Log _log{"AssetLoader"};

namespace dsr {
    namespace loader {

        static UPtr<bx::FileReaderI> sFileReaderHandle = UPtr<bx::FileReaderI>(new bx::CrtFileReader);

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
        UPtr<Sprite> loadSprite(T &obj) {
            Sprite sprite;

            auto sheetIt = obj.FindMember("sheet");
            if (sheetIt != obj.MemberEnd()) {
                sprite.sheetName = sheetIt->value.GetString();
            } else {
                _log.error("Could not load sprite sheet name.");
                return nullptr;
            }

            auto nameIt = obj.FindMember("name");
            if (nameIt != obj.MemberEnd()) {
                sprite.name = nameIt->value.GetString();
            } else {
                _log.error("Could not load sprite name.");
                return nullptr;
            }

            auto offsetIt = obj.FindMember("offset");
            if (offsetIt != obj.MemberEnd()) {
                sprite.offset.x = offsetIt->value[0].GetDouble();
                sprite.offset.y = offsetIt->value[1].GetDouble();
            } else {
                _log.error("Could not load sprite offset.");
                return nullptr;
            }

            auto sizeIt = obj.FindMember("size");
            if (sizeIt != obj.MemberEnd()) {
                sprite.size.x = sizeIt->value[0].GetDouble();
                sprite.size.y = sizeIt->value[1].GetDouble();
            } else {
                _log.error("Could not load sprite size.");
                return nullptr;
            }

            auto originIt = obj.FindMember("origin");
            if (originIt != obj.MemberEnd()) {
                sprite.origin.x = originIt->value[0].GetDouble();
                sprite.origin.y = originIt->value[1].GetDouble();
            } else {
                _log.warn("Sprite origin not specified. Defaulting to (0, 0).");
                sprite.origin = glm::vec2(0, 0);
            }

            return UPtr<Sprite>(new Sprite(sprite));
        }

        template<typename T>
        UPtr<StaticTile> loadStaticTile(T &json) {
            std::string name;
            glm::vec2 size;
            std::string spriteName;
            bool deadly;
            bool solid;
            Sprite *sprite;

            auto nameIt = json.FindMember("name");
            if (nameIt != json.MemberEnd()) {
                name = nameIt->value.GetString();
            } else {
                _log.error("Could not load the game object name.");
                return nullptr;
            }

            _log.info("Loading object ", name);

            auto sizeIt = json.FindMember("size");
            if (sizeIt != json.MemberEnd()) {
                size.x = sizeIt->value[0].GetDouble();
                size.y = sizeIt->value[1].GetDouble();
            } else {
                _log.error("Could not load the game object size.");
                return nullptr;
            }

            auto spriteIt = json.FindMember("sprite");
            if (spriteIt != json.MemberEnd()) {
                spriteName = nameIt->value.GetString();
            } else {
                _log.error("Could not load the game object sprite definition.");
                return nullptr;
            }

            sprite = Assets::instance().findSprite(spriteName);

            if (!sprite) {
                _log.error("Could not find a sprite called ", spriteName, ". Make sure to load the sprites first.");
                return nullptr;
            }

            auto deadlyIt = json.FindMember("deadly");
            if (deadlyIt != json.MemberEnd()) {
                deadly = deadlyIt->value.GetBool();
            } else {
                deadly = false;
            }

            auto solidIt = json.FindMember("solid");
            if (solidIt != json.MemberEnd()) {
                solid = solidIt->value.GetBool();
            } else {
                solid = false;
            }

            return UPtr<StaticTile>(new StaticTile(sprite, name, size, solid, deadly));
        }

        vector<UPtr<StaticTile>> loadStaticTiles(const std::string &jsonFilename) {
            JSONDocument doc(jsonFilename);
            vector<UPtr<StaticTile>> staticTiles;

            auto &json = doc.getDocument();

            U32 count = 0;

            _log.info("Loading objects from: ", jsonFilename);
            auto findIt = json.FindMember("static_tiles");
            if (findIt != json.MemberEnd()) {
                count = findIt->value.Size();
                for (U32 i = 0; i < count; ++i) {
                    auto staticTile = loadStaticTile(findIt->value[i]);
                    if (staticTile) {
                        staticTiles.push_back(std::move(staticTile));
                    }
                }
            } else {
                _log.warn("No definition for \"static_tiles\" was found in the script.");
            }

            _log.info("Successfuly loaded ", staticTiles.size(), " out of ", count, " static tiles.");

            return staticTiles;
        }

        std::vector<UPtr<Sprite>> loadSprites(const std::string &filename) {
            JSONDocument doc(filename);
            vector<UPtr<Sprite>> sprites;

            auto &json = doc.getDocument();

            U32 count = 0;

            _log.info("Loading sprites from: ", filename);
            auto findIt = json.FindMember("sprites");
            if (findIt != json.MemberEnd()) {
                count = findIt->value.Size();
                for (U32 i = 0; i < count; ++i) {
                    auto sprite = loadSprite(findIt->value[i]);
                    if (sprite) {
                        sprites.push_back(std::move(sprite));
                    }
                }
            } else {
                _log.warn("No definition for \"sprites\" was found in the script.");
            }

            _log.info("Successfuly loaded ", sprites.size(), " out of ", count, " sprites.");

            return sprites;
        }

        void loadParticleSystems(const std::string &jsonFilename) {
            _log.error(__FUNCTION__, " is not implemented");
        }

        UPtr<Texture> loadTexture(const std::string &filename) {
            const bgfx::Memory *mem = loadMem(sFileReaderHandle.get(), filename.c_str());

            bgfx::TextureInfo info;

            auto handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE, 0, &info);

            if (bgfx::isValid(handle)) {
                return UPtr<Texture>(new Texture(handle, info));
            }

            _log.warn("Could not load texture: ", filename);

            return nullptr;
        }

        static bgfx::ShaderHandle loadBgfxShader(const std::string &filePath) {
            return bgfx::createShader(loadMem(sFileReaderHandle.get(), filePath));
        }

        static bgfx::ProgramHandle loadBgfxProgram(const std::string &shaderName) {
            bgfx::ShaderHandle vsh = loadBgfxShader(shaderName + ".vert.bin");
            bgfx::ShaderHandle fsh = loadBgfxShader(shaderName + ".frag.bin");

            auto handle = bgfx::createProgram(vsh, fsh, true);

            return handle;
        }

        UPtr<Shader> loadShader(const std::string &filename) {
            bgfx::ProgramHandle handle = loadBgfxProgram(filename);
            if (bgfx::isValid(handle)) {
                _log.info("Successfuly loaded shader ", filename);
                return UPtr<Shader>(new Shader(handle));
            }

            _log.warn("Could not load shader ", filename);

            return nullptr;
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
}