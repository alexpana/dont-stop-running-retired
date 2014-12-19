#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include <memory>

namespace engine {
    class Vec2;

    enum class Key {
        SPACE,
        ESC,
        NONE
    };

    enum class MouseButton {
        LEFT,
        MIDDLE,
        RIGHT,
        NONE
    };

    // todo: add support for gamepads
    class Input {
    public:
        /**
        * Returns the current mouse position.
        */
        virtual Vec2 getMousePosition() = 0;

        /**
        * Returns true if the argument key was pressed this frame
        */
        virtual bool keyWasPressed(Key key) = 0;

        /**
        * Returns true if the argument key was released this frame
        */
        virtual bool keyWasReleased(Key key) = 0;

        /**
        * Returns true if the argument key is pressed
        */
        virtual bool keyIsDown(Key key) = 0;

        /**
        * Returns true if the argument mouse button is pressed
        */
        virtual bool mouseButtonIsDown(MouseButton key) = 0;

        /**
        * Call this once a frame to poll the input events
        * and update the state.
        */
        virtual void update() = 0;

        virtual ~Input() {
        }
    };
}
#pragma clang diagnostic pop