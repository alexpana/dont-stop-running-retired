#pragma once

#include <SDL.h>

namespace engine {
    class Timer {
    public:
        Timer() {
            m_startTime = SDL_GetTicks();
        }

        void restart() {
            m_startTime = SDL_GetTicks();
        }

        int getMilli() const {
            return SDL_GetTicks() - m_startTime;
        }

    private:
        int m_startTime;
    };
}