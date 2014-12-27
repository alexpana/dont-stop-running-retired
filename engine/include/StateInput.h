#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include "Constants.h"
#include "Memory.h"

namespace engine {
    class Vec2;

    class InputEventHandler;

    // todo: add support for gamepads
    struct StateInput {

        StateInput();

        ~StateInput();

        /**
        * Returns the current mouse position.
        */
        Vec2 getMousePosition();

        /**
        * Returns true if the argument key was pressed this frame
        */
        bool keyWasPressed(Key key);

        /**
        * Returns true if the argument key was released this frame
        */
        bool keyWasReleased(Key key);

        /**
        * Returns true if the argument key is pressed
        */
        bool keyIsDown(Key key);

        /**
        * Returns true if the argument mouse button is pressed
        */
        bool mouseButtonIsDown(MouseButton button);

        /**
        * Register this event handler with the input system to sync the
        */
        InputEventHandler *getEventHandler();

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}
#pragma clang diagnostic pop