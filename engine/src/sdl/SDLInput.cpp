#include "sdl/SDLInput.h"

#include <set>

#include <SDL_events.h>

#include "Log.h"
#include "Vec2.h"

namespace engine {

    struct SDLInput::Implementation {
        std::set<Key> pressedKeys;
        std::set<Key> recentlyPressedKeys;
        std::set<Key> recentlyReleasedKeys;

        uint8_t pressedMouseButtons = 0;
        uint8_t recentlyPressedMouseButtons = 0;
        uint8_t recentlyReleasedMouseButtons = 0;

        Vec2 mousePosition;
    };

    static uint8_t mouseButtonToInt(MouseButton mouseButton) {
        return (uint8_t) (1 << static_cast<int>(mouseButton));
    }

    static Key getKey(const SDL_Event &e) {
        if (!(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)) {
            return Key::UNKNOWN;
        }

        return static_cast<Key>(static_cast<int>(e.key.keysym.sym));
    }

    static MouseButton getMouseButton(const SDL_Event &e) {
        if (!(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)) {
            return MouseButton::NONE;
        }

        switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                return MouseButton::LEFT;
            case SDL_BUTTON_MIDDLE:
                return MouseButton::MIDDLE;
            case SDL_BUTTON_RIGHT:
                return MouseButton::RIGHT;
            default:
                return MouseButton::NONE;
        }
    }

    static bool isKeyDown(const SDL_Event &e) {
        return e.type == SDL_KEYDOWN;
    }

    static bool isKeyUp(const SDL_Event &e) {
        return e.type == SDL_KEYUP;
    }

    static bool isMouseMove(const SDL_Event &e) {
        return e.type == SDL_MOUSEMOTION;
    }

    static bool isMouseButtonDown(const SDL_Event &e) {
        return e.type == SDL_MOUSEBUTTONDOWN;
    }

    static bool isMouseButtonUp(const SDL_Event &e) {
        return e.type == SDL_MOUSEBUTTONUP;
    }

    Vec2 SDLInput::getMousePosition() {
        return impl->mousePosition;
    }

    bool SDLInput::keyWasPressed(Key sdlKey) {
        return impl->recentlyPressedKeys.find(sdlKey) != impl->recentlyPressedKeys.end();
    }

    bool SDLInput::keyWasReleased(Key sdlKey) {
        return impl->recentlyReleasedKeys.find(sdlKey) != impl->recentlyReleasedKeys.end();
    }

    bool SDLInput::keyIsDown(Key sdlKey) {
        return impl->pressedKeys.find(sdlKey) != impl->pressedKeys.end();
    }

    void SDLInput::update() {
        impl->recentlyPressedKeys.clear();
        impl->recentlyReleasedKeys.clear();

        impl->recentlyPressedMouseButtons = 0;
        impl->recentlyReleasedMouseButtons = 0;

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {

            if (isKeyDown(e)) {
                Key key = getKey(e);
                impl->recentlyPressedKeys.insert(key);
                impl->pressedKeys.insert(key);
            }

            if (isKeyUp(e)) {
                Key key = getKey(e);
                impl->recentlyReleasedKeys.insert(key);
                impl->pressedKeys.erase(key);
            }

            if (isMouseMove(e)) {
                impl->mousePosition = Vec2{(double) e.motion.x, (double) e.motion.y};
            }

            if (isMouseButtonDown(e)) {
                impl->pressedMouseButtons |= mouseButtonToInt(getMouseButton(e));
                impl->recentlyPressedMouseButtons |= mouseButtonToInt(getMouseButton(e));
            }

            if (isMouseButtonUp(e)) {
                impl->pressedMouseButtons &= ~(mouseButtonToInt(getMouseButton(e)));
                impl->recentlyReleasedMouseButtons |= mouseButtonToInt(getMouseButton(e));
            }
        }
    }

    bool SDLInput::mouseButtonIsDown(MouseButton button) {
        return (impl->pressedMouseButtons & mouseButtonToInt(button)) != 0;
    }

    SDLInput::SDLInput() {
        impl = std::unique_ptr<SDLInput::Implementation>(new SDLInput::Implementation);
    }
}