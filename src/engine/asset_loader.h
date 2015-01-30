#pragma once

#include <memory>
#include <string>
#include <vector>

#include "game_object.h"
#include "game_objects/static_tile.h"
#include "log.h"
#include "level_map.h"
#include "shader.h"
#include "texture.h"
#include "types.h"

namespace dsr {
    namespace loader {
        std::vector<UPtr<StaticTile>> loadStaticTiles(const std::string &filename);

        std::vector<UPtr<Sprite>> loadSprites(const std::string &filename);

        void loadParticleSystems(const std::string &jsonFilename);

        UPtr<Texture> loadTexture(const std::string &filename);

        UPtr<Shader> loadShader(const std::string &filename);

        bool loadMapLevel(const std::string &filename, LevelMap &levelMap);

        void *loadRawFile(const std::string &filename, U32 *size);
    }
}