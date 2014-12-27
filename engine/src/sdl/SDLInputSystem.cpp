#include "sdl/SDLInputSystem.h"

#include <algorithm>
#include <iostream>
#include <set>

#include <SDL_events.h>

namespace engine {

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

    struct SDLInputSystem::Implementation {
        Vec2 previousMousePosition;
    };

    void SDLInputSystem::doUpdate() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {

            if (isKeyDown(e)) {
                KeyEvent event{getKey(e)};
                notifyKeyPressed(event);
            }

            if (isKeyUp(e)) {
                KeyEvent event{getKey(e)};
                notifyKeyReleased(event);
            }

            if (isMouseMove(e)) {
                Vec2 mousePosition = Vec2{(double) e.motion.x, (double) e.motion.y};
                MouseMoveEvent event{mousePosition, mousePosition - impl->previousMousePosition};
                impl->previousMousePosition = mousePosition;
                notifyMouseMoved(event);
            }

            if (isMouseButtonDown(e)) {
                MouseButtonEvent event{getMouseButton(e)};
                notifyMouseButtonPressed(event);
            }

            if (isMouseButtonUp(e)) {
                MouseButtonEvent event{getMouseButton(e)};
                notifyMouseButtonReleased(event);
            }
        }
    }

    SDLInputSystem::SDLInputSystem() {
        impl = std::make_unique<Implementation>();
    }

    SDLInputSystem::~SDLInputSystem() {
    }
}

