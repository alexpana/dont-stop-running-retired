#pragma once

#include <memory>

#include "InputSystem.h"

namespace engine {
    class SDLInputSystem : public InputSystem {
    public:
        SDLInputSystem();

        ~SDLInputSystem();

    protected:
        void doUpdate() override;

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}