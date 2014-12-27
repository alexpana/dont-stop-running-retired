#include "StateInput.h"

#include <set>

#include "InputSystem.h"

namespace engine {

    static uint8_t mouseButtonToInt(MouseButton mouseButton) {
        return (uint8_t) (1 << static_cast<int>(mouseButton));
    }

    struct StateInput::Implementation : public InputEventHandler {
        void updateStarted() override;

        void updateEnded() override;

        bool keyPressed(const KeyEvent &event) override;

        bool keyReleased(const KeyEvent &event) override;

        bool mouseMoved(const MouseMoveEvent &event) override;

        bool mouseButtonPressed(const MouseButtonEvent &event) override;

        bool mouseButtonReleased(const MouseButtonEvent &event) override;

        std::set<Key> pressedKeys;
        std::set<Key> recentlyPressedKeys;
        std::set<Key> recentlyReleasedKeys;

        uint8_t pressedMouseButtons = 0;
        uint8_t recentlyPressedMouseButtons = 0;
        uint8_t recentlyReleasedMouseButtons = 0;

        Vec2 mousePosition;
    };

    Vec2 StateInput::getMousePosition() {
        return impl->mousePosition;
    }

    bool StateInput::keyWasPressed(Key key) {
        return impl->recentlyPressedKeys.find(key) != impl->recentlyPressedKeys.end();
    }

    bool StateInput::keyWasReleased(Key key) {
        return impl->recentlyReleasedKeys.find(key) != impl->recentlyReleasedKeys.end();
    }

    bool StateInput::keyIsDown(Key key) {
        return impl->pressedKeys.find(key) != impl->pressedKeys.end();
    }

    bool StateInput::mouseButtonIsDown(MouseButton button) {
        return (impl->pressedMouseButtons & mouseButtonToInt(button)) != 0;
    }

    InputEventHandler *StateInput::getEventHandler() {
        return impl.get();
    }

    StateInput::StateInput() {
        impl = std::make_unique<StateInput::Implementation>();
    }

    StateInput::~StateInput() {
    }

    bool StateInput::Implementation::keyPressed(const KeyEvent &event) {
        recentlyPressedKeys.insert(event.key);
        pressedKeys.insert(event.key);
    }

    bool StateInput::Implementation::keyReleased(const KeyEvent &event) {
        recentlyReleasedKeys.insert(event.key);
        pressedKeys.erase(event.key);
    }

    bool StateInput::Implementation::mouseMoved(const MouseMoveEvent &event) {
        mousePosition = event.mousePosition;
    }

    bool StateInput::Implementation::mouseButtonPressed(const MouseButtonEvent &event) {
        pressedMouseButtons |= mouseButtonToInt(event.mouseButton);
        recentlyPressedMouseButtons |= mouseButtonToInt(event.mouseButton);
    }

    bool StateInput::Implementation::mouseButtonReleased(const MouseButtonEvent &event) {
        pressedMouseButtons &= ~(mouseButtonToInt(event.mouseButton));
        recentlyReleasedMouseButtons |= mouseButtonToInt(event.mouseButton);
    }

    void StateInput::Implementation::updateStarted() {
        recentlyPressedKeys.clear();
        recentlyReleasedKeys.clear();
        recentlyPressedMouseButtons = 0;
        recentlyReleasedMouseButtons = 0;
    }

    void StateInput::Implementation::updateEnded() {
    }
}