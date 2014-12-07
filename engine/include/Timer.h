#pragma once

#include <stdint.h>

namespace engine {
    class Timer {
    public:
        Timer();

        void restart();

        double seconds();

    private:
        uint64_t timestamp;
    };
}