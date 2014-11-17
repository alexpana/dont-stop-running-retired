#pragma once

#include <string>

#include <Windows.h>

namespace engine {

class Filesystem {

public:

    std::string getCurrentWorkingDirectory() 
    {
        char buffer[128];
        GetCurrentDirectory(128, buffer);
        return buffer;
    }

};

}