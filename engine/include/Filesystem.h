#pragma once

#include <string>

#include <Windows.h>

namespace engine {
    class Filesystem {
    public:
        std::string getCurrentWorkingDirectory();
    };

}