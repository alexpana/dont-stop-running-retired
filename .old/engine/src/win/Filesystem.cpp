#include "Filesystem.h"

static char buffer[128];

namespace engine {
    std::string Filesystem::getCurrentWorkingDirectory() {
        GetCurrentDirectory(128, buffer);

        return std::string{buffer};
    };
}