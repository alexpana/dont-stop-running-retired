#include "Timer.h"

#include "windows.h"

static LARGE_INTEGER TIMER_FREQUENCY = {0};

namespace engine {
    Timer::Timer() {
        if (TIMER_FREQUENCY.QuadPart == 0) {
            QueryPerformanceFrequency(&TIMER_FREQUENCY);
        }

        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timestamp));
    }



    double Timer::seconds() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return (double)(currentTime.QuadPart - timestamp) / TIMER_FREQUENCY.QuadPart;
    }

    void Timer::restart() {
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timestamp));
    }

}