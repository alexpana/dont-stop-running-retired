#include <SDL_keycode.h>
#include <SDL_events.h>
#include "input.h"

namespace dsr {
    namespace input {
        static std::function<void(Key, InputAction)> keyCallback = nullptr;
        static std::function<void(Key, InputAction)> charCallback = nullptr;
        static std::function<void(MouseButton, InputAction)> mouseButtonCallback = nullptr;
        static std::function<void(F32, F32)> mouseWheelCallback = nullptr;

        struct MouseState {
            F32 position[2];
            bool leftDown;
            bool leftPressed;
            bool leftReleased;
            bool rightDown;
            bool rightPressed;
            bool rightReleased;
            bool middleDown;
            bool middlePressed;
            bool middleReleased;
            F32 wheel[2];
        };

        struct KeyboardState {
            Key pressedKey;
            Key releasedKey;
            bool keysDown[256];
        };

        static MouseState sMouseState;
        static KeyboardState sKeyboardState;
        static bool sQuitRequested = false;

        struct SpecialKey {
            enum {
                _FIRST = 0,
                ESCAPE,
                ENTER,
                TAB,
                BACKSPACE,
                DELETE,
                RIGHT,
                LEFT,
                UP,
                DOWN,
                HOME,
                END,
                _LAST
            };
        };

        static bool isSpecialKey(U8 key) {
            return (key > SpecialKey::_FIRST && key < SpecialKey::_LAST);
        }

        void update() {
            SDL_Event event;

            sKeyboardState.pressedKey = 0;
            sMouseState.leftPressed = sMouseState.leftReleased = false;
            sMouseState.middlePressed = sMouseState.middleReleased = false;
            sMouseState.rightPressed = sMouseState.rightReleased = false;

            // input
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    sQuitRequested = true;
                }

                if (event.type == SDL_MOUSEMOTION) {
                    sMouseState.position[0] = event.motion.x;
                    sMouseState.position[1] = event.motion.y;
                }

                if (event.type == SDL_MOUSEWHEEL) {
                    sMouseState.wheel[0] = event.wheel.x;
                    sMouseState.wheel[1] = event.wheel.y;
                    mouseWheelCallback(sMouseState.wheel[0], sMouseState.wheel[1]);
                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        sMouseState.leftDown = true;
                        sMouseState.leftPressed = true;
                        mouseButtonCallback(MouseButton::LEFT, InputAction::PRESSED);
                    }

                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        sMouseState.rightDown = true;
                        sMouseState.rightPressed = true;
                        mouseButtonCallback(MouseButton::RIGHT, InputAction::PRESSED);
                    }

                    if (event.button.button == SDL_BUTTON_MIDDLE) {
                        sMouseState.middleDown = true;
                        sMouseState.middlePressed = true;
                        mouseButtonCallback(MouseButton::MIDDLE, InputAction::PRESSED);
                    }
                }

                if (event.type == SDL_MOUSEBUTTONUP) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        sMouseState.leftDown = false;
                        sMouseState.leftReleased = true;
                        mouseButtonCallback(MouseButton::LEFT, InputAction::RELEASED);
                    }

                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        sMouseState.rightDown = false;
                        sMouseState.rightReleased = true;
                        mouseButtonCallback(MouseButton::RIGHT, InputAction::RELEASED);
                    }

                    if (event.button.button == SDL_BUTTON_MIDDLE) {
                        sMouseState.middleDown = false;
                        sMouseState.middleReleased = true;
                        mouseButtonCallback(MouseButton::MIDDLE, InputAction::RELEASED);
                    }
                }

                if (event.type == SDL_KEYDOWN) {
                    U8 key = fromSdlKey(event.key.keysym.sym);

                    sKeyboardState.keysDown[key] = true;
                    sKeyboardState.pressedKey = key;

                    if (isSpecialKey(key)) {
                        keyCallback(key, InputAction::PRESSED);
                    } else {
                        charCallback(key, InputAction::PRESSED);
                    }
                }

                if (event.type == SDL_KEYUP) {
                    U8 key = fromSdlKey(event.key.keysym.sym);

                    sKeyboardState.keysDown[key] = false;
                    sKeyboardState.releasedKey = key;

                    if (isSpecialKey(key)) {
                        keyCallback(key, InputAction::RELEASED);
                    } else {
                        charCallback(key, InputAction::RELEASED);
                    }
                }
            }
        }

        void setCharCallback(std::function<void(Key, InputAction)> callback) {
            charCallback = callback;
        }

        void setKeyCallback(std::function<void(Key, InputAction)> callback) {
            keyCallback = callback;
        }

        void setMouseButtonCallback(std::function<void(MouseButton, InputAction)> callback) {
            mouseButtonCallback = callback;
        }

        void setMouseWheelCallback(std::function<void(F32, F32)> callback) {
            mouseWheelCallback = callback;
        }

        F32 mouseX() {
            return sMouseState.position[0];
        }

        F32 mouseY() {
            return sMouseState.position[1];
        }

        F32 wheelX() {
            return 0;
        }

        F32 wheelY() {
            return 0;
        }

        bool keyDown(Key key) {
            return sKeyboardState.keysDown[key];
        }

        bool mouseButtonDown(MouseButton button) {
            switch (button) {
                case MouseButton::LEFT:
                    return sMouseState.leftDown;
                case MouseButton::MIDDLE:
                    return sMouseState.middleDown;
                case MouseButton::RIGHT:
                    return sMouseState.rightDown;
            }
        }

        bool mouseButtonPressed(MouseButton button) {
            switch (button) {
                case MouseButton::LEFT:
                    return sMouseState.leftPressed;
                case MouseButton::MIDDLE:
                    return sMouseState.middlePressed;
                case MouseButton::RIGHT:
                    return sMouseState.rightPressed;
            }
        }

        bool mouseButtonReleased(MouseButton button) {
            switch (button) {
                case MouseButton::LEFT:
                    return sMouseState.leftReleased;
                case MouseButton::MIDDLE:
                    return sMouseState.middleReleased;
                case MouseButton::RIGHT:
                    return sMouseState.rightReleased;
            }
        }

        Key fromSdlKey(S32 keycode) {
            switch (keycode) {
                case SDLK_LEFT:
                    return SpecialKey::LEFT;
                case SDLK_RIGHT:
                    return SpecialKey::RIGHT;
                case SDLK_UP:
                    return SpecialKey::UP;
                case SDLK_DOWN:
                    return SpecialKey::DOWN;
                case SDLK_DELETE:
                    return SpecialKey::DELETE;
                case SDLK_BACKSPACE:
                    return SpecialKey::BACKSPACE;
                case SDLK_RETURN:
                    return SpecialKey::ENTER;
                case SDLK_TAB:
                    return SpecialKey::TAB;
                case SDLK_HOME:
                    return SpecialKey::HOME;
                case SDLK_END:
                    return SpecialKey::END;
                case SDLK_ESCAPE:
                    return SpecialKey::ESCAPE;
                default:
                    return (U8) (keycode & 0xff);
            }
        }

        bool frameCloseRequested() {
            return sQuitRequested;
        }

        bool keyPressed(Key key) {
            return key == sKeyboardState.pressedKey;
        }

        bool keyReleased(Key key) {
            return key == sKeyboardState.releasedKey;
        }
    }
}