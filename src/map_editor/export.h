#pragma once

#include "../engine/level_map.h"

namespace dsr {
    void exportLevelMap(const std::string &filename, const LevelMap &levelMap);
}