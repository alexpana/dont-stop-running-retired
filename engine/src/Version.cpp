#include "Version.h"

#include <ostream>

namespace engine {

    std::ostream &operator<<(std::ostream &lhs, Version &rhs) {
        return lhs << rhs.major << "." << rhs.minor << "." << rhs.patch;
    }

    void getVersion(Version *version) {
        if (version == nullptr) {
            return;
        }

        version->major = VERSION_MAJOR;
        version->minor = VERSION_MINOR;
        version->patch = VERSION_PATCH;
    }
}