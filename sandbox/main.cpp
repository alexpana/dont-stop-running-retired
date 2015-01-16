#include "bgfx.h"

#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main() {
    bgfx::init();
    bgfx::reset(200, 200, BGFX_RESET_VSYNC);

    // Set view 0 clear state.
    bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x303030ff,
            1.0f,
            0);

    while (true) {
        bgfx::setViewRect(0, 0, 0, 200, 200);

        bgfx::submit(0);

        bgfx::dbgTextPrintf(0, 1, 0x4f, "Hello World!");

        bgfx::frame();
    }
}
#pragma clang diagnostic pop