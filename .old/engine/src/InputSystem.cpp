#include "InputSystem.h"

#include <algorithm>
#include <set>

namespace engine {

    struct PrioritizedInputEventHandler {
        InputEventHandler *eventHandler;
        int priority;

        bool operator>(const PrioritizedInputEventHandler &rhs) const {
            return priority > rhs.priority;
        }
    };

    struct InputSystem::Implementation {
        std::set<PrioritizedInputEventHandler, std::greater<PrioritizedInputEventHandler>> listeners;

        void notifyUpdateStarted();

        void notifyUpdateEnded();
    };

    void InputSystem::update() {
        impl->notifyUpdateStarted();

        doUpdate();

        impl->notifyUpdateEnded();
    }

    void InputSystem::addEventHandler(InputEventHandler *handler, int priority) {
        impl->listeners.insert(PrioritizedInputEventHandler{handler, priority});
    }

    void InputSystem::removeEventHandler(InputEventHandler *handler) {
        auto findIt = std::find_if(
                impl->listeners.begin(),
                impl->listeners.end(),
                [handler](const PrioritizedInputEventHandler &item) {
                    return item.eventHandler == handler;
                });

        if (findIt != impl->listeners.end()) {
            impl->listeners.erase(findIt);
        }
    }

    InputSystem::InputSystem() :
            impl{std::make_unique<InputSystem::Implementation>()} {
    }

    InputSystem::~InputSystem() {
    }

    bool InputSystem::notifyKeyPressed(const KeyEvent &event) {
        bool handled = false;
        for (auto &eventHandler : impl->listeners) {
            handled = eventHandler.eventHandler->keyPressed(event);
            if (handled) {
                break;
            }
        }
        return handled;
    }

    bool InputSystem::notifyKeyReleased(const KeyEvent &event) {
        bool handled = false;
        for (auto &eventHandler : impl->listeners) {
            handled = eventHandler.eventHandler->keyReleased(event);
            if (handled) {
                break;
            }
        }
        return handled;
    }

    bool InputSystem::notifyMouseMoved(const MouseMoveEvent &event) {
        bool handled = false;
        for (auto &eventHandler : impl->listeners) {
            handled = eventHandler.eventHandler->mouseMoved(event);
            if (handled) {
                break;
            }
        }
        return handled;
    }

    bool InputSystem::notifyMouseButtonPressed(const MouseButtonEvent &event) {
        bool handled = false;
        for (auto &eventHandler : impl->listeners) {
            handled = eventHandler.eventHandler->mouseButtonPressed(event);
            if (handled) {
                break;
            }
        }
        return handled;
    }

    bool InputSystem::notifyMouseButtonReleased(const MouseButtonEvent &event) {
        bool handled = false;
        for (auto &eventHandler : impl->listeners) {
            handled = eventHandler.eventHandler->mouseButtonReleased(event);
            if (handled) {
                break;
            }
        }
        return handled;
    }

    void InputSystem::Implementation::notifyUpdateStarted() {
        for (auto &eventHandler : listeners) {
            eventHandler.eventHandler->updateStarted();
        }
    }

    void InputSystem::Implementation::notifyUpdateEnded() {
        for (auto &eventHandler : listeners) {
            eventHandler.eventHandler->updateEnded();
        }
    }
}