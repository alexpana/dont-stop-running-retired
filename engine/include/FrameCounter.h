#pragma once

#include "Game.h"

namespace engine {
    class FrameCounter {
    public:
        explicit FrameCounter(const engine::GamePtr game) :
                m_game(game),
                lastFrameCount(m_game->getFrameCount()) {
        }

        int getMilli() {
            return m_timer.getMilli();
        }

        void restart() {
            m_timer.restart();
            lastFrameCount = m_game->getFrameCount();
        }

        double getFPS() {
            return static_cast<double>(m_game->getFrameCount() - lastFrameCount) * 1000 / m_timer.getMilli();
        }

    private:
        const engine::GamePtr m_game;

        engine::Timer m_timer;

        int lastFrameCount = 0;
    };
}