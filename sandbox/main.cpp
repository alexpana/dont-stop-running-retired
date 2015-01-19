#include <SDL2/SDL.h>
#include <bx/bx.h>
#include <bx/fpumath.h>
#include <bx/timer.h>
#include <bx/readerwriter.h>
#include <bgfx.h>
#include <bgfxplatform.h>
#include <iostream>

#include "bgfx_utils.h"

using namespace std;

struct PosTexVertex {
    float x, y, z;
    float u, v;

    static bgfx::VertexDecl getDecl() {
        if (!declarationInit) {
            declarationInit = true;
            declaration.begin()
                    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                    .end();
        }
        return declaration;
    }

private:
    static bgfx::VertexDecl declaration;
    static bool declarationInit;
};

bgfx::VertexDecl PosTexVertex::declaration;
bool PosTexVertex::declarationInit = false;

//static PosTexVertex sQuadVertices[4] = {
//        {-1.0f,  1.0f, 0.0f, 0.0f, 1.0f},
//        {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
//        { 1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
//        { 1.0f,  1.0f, 0.0f, 1.0f, 1.0f}
//};

static PosTexVertex sQuadVertices[4] = {
        {10.0f,  10.0f, 0.0f, 0.0f, 1.0f},
        {10.0f,  100.0f, 0.0f, 0.0f, 0.0f},
        {110.0f, 100.0f, 0.0f, 1.0f, 0.0f},
        {110.0f, 10.0f, 0.0f, 1.0f, 1.0f}
};

static const uint16_t sQuadIndices[6] = {
        0, 1, 2,
        0, 2, 3
};

int main() {
    uint64_t timerFrequency = bx::getHPFrequency();
    cout << "HP timer frequency: " << timerFrequency << endl;
    uint16_t width = 800;
    uint16_t height = 600;

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

    bgfx_utils::init();

    // load texture
    bgfx::TextureInfo textureInfo;
    auto textureHandle = bgfx_utils::loadTexture("data/bg_64_64.dds", BGFX_TEXTURE_NONE, 0, &textureInfo);
    cout << "Texture size: " << textureInfo.width << "x" << textureInfo.height << " storage: " << textureInfo.storageSize << endl;

    bgfx::VertexBufferHandle quadVB = bgfx::createVertexBuffer(bgfx::makeRef(sQuadVertices, sizeof(sQuadVertices)), PosTexVertex::getDecl());

    bgfx::IndexBufferHandle quadIB = bgfx::createIndexBuffer(bgfx::makeRef(sQuadIndices, sizeof(sQuadIndices)));


    float view[16];
    bx::mtxIdentity(view);

    float proj[16];
    bx::mtxOrtho(proj, 0, width, height, 0, 1, 100);

    bgfx::setViewTransform(0, view, proj);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    bgfx::ProgramHandle program = bgfx_utils::loadProgram("default");

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            bgfx::submit(0);

            bgfx::setProgram(program);

            bgfx::setVertexBuffer(quadVB);

            bgfx::setIndexBuffer(quadIB);

//            bgfx::setImage(0, textureHandle);

            bgfx::submit(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(1, 2, 0x0B, "FPS: unknown");

            bgfx::frame();
        }
    }

    bgfx::destroyProgram(program);

    return 0;
}
