#pragma once

#include <string>
#include <vector>

#include "game_object.h"
#include "log.h"
#include "level_map.h"
#include "shader.h"
#include "texture.h"

namespace dsr {
    std::vector<GameObject> loadObjects(const std::string &jsonFilename);

    void loadSprites(const std::string &jsonFilename);

    void loadParticleSystems(const std::string &jsonFilename);

    bool loadTexture(const std::string &filename, Texture &texture);

    bool loadShader(const std::string &filename, Shader &shader);

    bool loadMapLevel(const std::string &filename, LevelMap &levelMap);

    void *loadRawFile(const std::string &filename, U32 *size);
}