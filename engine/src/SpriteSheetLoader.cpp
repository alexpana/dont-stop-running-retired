#include "SpriteSheetLoader.h"

#include <iostream>
#include <stdio.h>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "Log.h"
#include "Memory.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "TextureLoader.h"

using namespace rapidjson;
using namespace std;

static engine::Log _log{"SpriteSheetLoader"};

namespace engine {

    static const char* TEXTURE_KEY = "texture";
    static const char* SPRITES_KEY = "sprites";
    static const char* ORIGIN_KEY = "origin";

    string getTextureName(Document &json) {
        auto findIt = json.FindMember(TEXTURE_KEY);
        if (findIt != json.MemberEnd()) {
            return findIt->value.GetString();
        }
        return "";
    }



    class JSONDocument {
    public:
        JSONDocument(const string filename) {
            fileHandle = fopen(filename.c_str(), "rb");

            readBuffer = new char[READ_BUFFER_SIZE];

            FileReadStream is(fileHandle, readBuffer, READ_BUFFER_SIZE);

            document.ParseStream(is);
        }

        ~JSONDocument() {
            delete[] readBuffer;

            fclose(fileHandle);
        }

        Document &getDocument() {
            return document;
        }

    private:
        Document document;
        char *readBuffer;
        FILE *fileHandle;

        static constexpr unsigned int READ_BUFFER_SIZE = 65536;
    };

    template<typename Encoding>
    Rect2 getSpriteBounds(GenericValue<Encoding> &value, bool &error) {
        double x = 0, y = 0, w = 0, h = 0;

        auto findX = value.FindMember("x");
        findX != value.MemberEnd() ? x = findX->value.GetDouble() : error = true;

        auto findY = value.FindMember("y");
        findY != value.MemberEnd() ? y = findY->value.GetDouble() : error = true;

        auto findW = value.FindMember("w");
        findW != value.MemberEnd() ? w = findW->value.GetDouble() : error = true;

        auto findH = value.FindMember("h");
        findH != value.MemberEnd() ? h = findH->value.GetDouble() : error = true;

        return Rect2{x, y, w, h};
    }

    template<typename Encoding>
    Vec2 getSpriteOrigin(GenericValue<Encoding> &value, bool &error) {
        auto findOrigin = value.FindMember(ORIGIN_KEY);
        GenericValue<Encoding> origin;

        if (findOrigin == value.MemberEnd()) {
            error = true;
            return Vec2{0, 0};
        } else {
            origin = findOrigin->value;
        }

        double x = 0, y = 0;

        auto findX = origin.FindMember("x");
        findX != origin.MemberEnd() ? x = findX->value.GetDouble() : error = true;

        auto findY = origin.FindMember("y");
        findY != origin.MemberEnd() ? y = findY->value.GetDouble() : error = true;

        return Vec2{x, y};
    }

    unique_ptr<SpriteSheet> SpriteSheetLoader::load(const string descriptionFile) {
        JSONDocument jsonDocument{descriptionFile};

        auto& json = jsonDocument.getDocument();

        unique_ptr<SpriteSheet> spriteSheet = std::make_unique<SpriteSheet>();

        auto textureName = getTextureName(json);

        // todo: handle relative paths

        auto spriteSheetTexture = textureLoader->load(textureName);

        spriteSheet->setTexture(std::move(spriteSheetTexture));

        auto findSpritesIt = json.FindMember(SPRITES_KEY);

        if (findSpritesIt != json.MemberEnd()) {
            auto sprites = std::move(findSpritesIt->value);
            for (auto spritesIt = sprites.MemberBegin(); spritesIt != sprites.MemberEnd(); ++spritesIt) {
                string spriteName = spritesIt->name.GetString();

                bool error = false;
                Rect2 spriteBounds = getSpriteBounds(spritesIt->value, error);
                if (error) {
                    // todo: handle error
                }

                error = false;
                Vec2 spriteOrigin = getSpriteOrigin(spritesIt->value, error);
                if (error) {
                    // todo: handle error
                }

                spriteSheet->addSprite(spriteName, std::make_unique<Sprite>(spriteBounds, spriteOrigin, spriteSheet->getTexture()));
            }
        }

        return spriteSheet;
    };
}