#pragma once

#include <stdint.h>

namespace engine {
    class Timer {
    public:
        Timer();

        void restart();

        double seconds() const;

    private:
        uint64_t timestamp;
    };
}