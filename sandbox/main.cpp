#include "SDL2/SDL.h"
#include "bgfx.h"
#include "bgfxplatform.h"
#include <iostream>

using namespace std;

int main() {
    uint32_t width = 1280;
    uint32_t height = 720;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *wnd = SDL_CreateWindow("hello", 0, 0, width, height, SDL_WINDOW_SHOWN);


    bgfx::sdlSetWindow(wnd);
    bgfx::init();
    bgfx::reset(width, height, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    // Set view 0 clear state.
    bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x3030ffff,
            1.0f,
            0);

    while (true) {
        bgfx::setViewRect(0, 0, 0, width, height);

        bgfx::submit(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(1, 1, 0x6f, "Hello World!");
        bgfx::dbgTextPrintf(1, 2, 0x4f, "I'm a BGFX Window!");

        bgfx::frame();
    }

    return 0;
}
