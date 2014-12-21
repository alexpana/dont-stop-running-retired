#pragma once

#include "Engine.h"

namespace engine {
    class FPSCounter {
    public:
        explicit FPSCounter(const engine::Engine* game) :
                engine(game),
                lastFrameCount(engine->getFrameCount()) {
        }

        double seconds() {
            return timer.seconds();
        }

        void restart() {
            timer.restart();
            lastFrameCount = engine->getFrameCount();
        }

        double getFPS() {
            return static_cast<double>(engine->getFrameCount() - lastFrameCount) / timer.seconds();
        }

    private:
        const engine::Engine *engine;

        engine::Timer timer;

        int lastFrameCount = 0;
    };
}