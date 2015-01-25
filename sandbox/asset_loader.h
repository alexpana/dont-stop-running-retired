#pragma once

#include <string>
#include <vector>

#include "game_object.h"
#include "log.h"

namespace dsr {
    std::vector<GameObject> loadObjects(const std::string &jsonFilename);

    void loadSprites(const std::string &jsonFilename);

    void loadParticleSystems(const std::string &jsonFilename);
}