#pragma once

#include <string>

#include <Windows.h>

namespace engine {
    class Filesystem {
    public:
        std::string getCurrentWorkingDirectory() {
            char *buffer = new char[128];

            // TODO: port this
            GetCurrentDirectory(128, buffer);

            return buffer;
        }
    };

}