#pragma once

#include <memory>

#include "Constants.h"
#include "Vec2.h"

namespace engine {

    struct KeyEvent {
        const Key key;
    };

    struct MouseMoveEvent {
        const Vec2 mousePosition;
        const Vec2 moveDelta;
    };

    struct MouseButtonEvent {
        const MouseButton mouseButton;
    };

    /**
    * An event handler is notified by the input system when an input event occurred.
    * Event handlers are required to return TRUE if that event was consumed by the handler.
    */
    struct InputEventHandler {
        virtual void updateStarted() = 0;

        virtual void updateEnded() = 0;

        virtual bool keyPressed(const KeyEvent &event) = 0;

        virtual bool keyReleased(const KeyEvent &event) = 0;

        virtual bool mouseMoved(const MouseMoveEvent &event) = 0;

        virtual bool mouseButtonPressed(const MouseButtonEvent &event) = 0;

        virtual bool mouseButtonReleased(const MouseButtonEvent &event) = 0;

        virtual ~InputEventHandler() {
        };
    };

    struct InputSystem {
        /**
        * Call this once a frame to poll the input events.
        */
        void update();

        virtual void addEventHandler(InputEventHandler *handler, int priority);

        virtual void removeEventHandler(InputEventHandler *handler);

        InputSystem();

        virtual ~InputSystem();

    protected:
        virtual void doUpdate() = 0;

        bool notifyKeyPressed(const KeyEvent &event);

        bool notifyKeyReleased(const KeyEvent &event);

        bool notifyMouseMoved(const MouseMoveEvent &event);

        bool notifyMouseButtonPressed(const MouseButtonEvent &event);

        bool notifyMouseButtonReleased(const MouseButtonEvent &event);

    private:
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    };
}