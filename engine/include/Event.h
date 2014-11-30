#pragma once

#include <SDL.h>


namespace engine {
    class Event {
    public:
        static bool isMouseDown(const SDL_Event &e, int mouseButton) {
            return e.type == SDL_MOUSEBUTTONDOWN && e.button.button == mouseButton;
        }

        static bool isQuit(const SDL_Event &e) {
            return e.type == SDL_QUIT;
        }

        static bool isKeyDown(const SDL_Event &e, int key) {
            return e.type == SDL_KEYDOWN && e.key.keysym.sym == key;
        }

        static bool isKeyUp(const SDL_Event &e, int key) {
            return e.type == SDL_KEYUP && e.key.keysym.sym == key;
        }
    };

}