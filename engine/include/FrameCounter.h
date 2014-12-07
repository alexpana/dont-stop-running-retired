#pragma once

#include "Game.h"

namespace engine {
    class FrameCounter {
    public:
        explicit FrameCounter(const engine::GamePtr game) :
                m_game(game),
                lastFrameCount(m_game->getFrameCount()) {
        }

        int seconds() {
            return m_timer.seconds();
        }

        void restart() {
            m_timer.restart();
            lastFrameCount = m_game->getFrameCount();
        }

        double getFPS() {
            return static_cast<double>(m_game->getFrameCount() - lastFrameCount) / m_timer.seconds();
        }

    private:
        const engine::GamePtr m_game;

        engine::Timer m_timer;

        int lastFrameCount = 0;
    };
}