#pragma once

#include <iosfwd>

namespace engine {

    static const unsigned int VERSION_MAJOR = 1;

    static const unsigned int VERSION_MINOR = 2;

    static const unsigned int VERSION_PATCH = 0;

    struct Version {
        unsigned int major;
        unsigned int minor;
        unsigned int patch;
    };

    std::ostream &operator<<(std::ostream &lhs, Version &rhs);

    void getVersion(Version *version);
}
