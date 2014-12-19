#pragma once

#include "Input.h"

namespace engine {

    class SDLInput : public Input {
    public:
        SDLInput();

        Vec2 getMousePosition() override;

        bool keyWasPressed(Key key) override;

        bool keyWasReleased(Key key) override;

        bool keyIsDown(Key key) override;

        bool mouseButtonIsDown(MouseButton button) override;

        void update() override;

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}