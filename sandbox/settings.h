#pragma once

#include <stdint.h>

namespace dsr {
    /**
    * Maximum number of particles that can be alive at any time.
    *
    * Default: 128
    */
    static constexpr uint32_t kParticleLimit = 512;
};