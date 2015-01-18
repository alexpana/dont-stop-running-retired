#include <SDL2/SDL.h>
#include <bx/bx.h>
#include <bx/timer.h>
#include <bx/readerwriter.h>
#include <bgfx.h>
#include <bgfxplatform.h>
#include <iostream>

using namespace std;

static bx::FileReaderI *sFileReader = nullptr;

static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath) {
    std::cout << "loadMem(" << _filePath << ")" << std::endl;
    if (0 == bx::open(_reader, _filePath)) {
        uint32_t size = (uint32_t) bx::getSize(_reader);
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        bx::read(_reader, mem->data, size);
        bx::close(_reader);
        mem->data[mem->size - 1] = '\0';
        std::cout << "Mem[" << &mem->data << "].size = " << mem->size << std::endl;
        return mem;
    } else {
        std::cout << "Could not load " << _filePath << std::endl;
    }

    return nullptr;
}

bgfx::TextureHandle loadTexture(const char *_name,
        uint32_t _flags = BGFX_TEXTURE_NONE,
        uint8_t _skip = 0,
        bgfx::TextureInfo *_info = nullptr) {

    const bgfx::Memory *mem = loadMem(sFileReader, _name);

    return bgfx::createTexture(mem, _flags, _skip, _info);
}

int main() {
    uint64_t timerFrequency = bx::getHPFrequency();
    cout << "HP timer frequency: " << timerFrequency << endl;
    uint16_t width = 800;
    uint16_t height = 600;

    sFileReader = new bx::CrtFileReader;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *wnd = SDL_CreateWindow(
            "BGFX sandbox",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    bgfx::sdlSetWindow(wnd);
    bgfx::init();
    bgfx::reset(width, height, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Set view 0 clear state.
    bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x313233ff,
            1.0f,
            0);

    // load texture
//    bgfx::TextureInfo textureInfo;
//    auto textureHandle = loadTexture("data/fieldstone-rgba.tga", BGFX_TEXTURE_NONE, 0, &textureInfo);
//    cout << "Texture size: " << textureInfo.width << "x" << textureInfo.height << " storage: " << textureInfo.storageSize << endl;

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            bgfx::setViewRect(0, 0, 0, width, height);

            bgfx::submit(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(1, 2, 0x0B, "FPS: unknown");

            bgfx::frame();
        }
    }

    delete sFileReader;

    return 0;
}
