#include "engine/json_reader.h"

#include <SDL2/SDL.h>
#include <bx/bx.h>
#include <bx/fpumath.h>
#include <bx/timer.h>
#include <bx/readerwriter.h>
#include <bgfx.h>
#include <bgfxplatform.h>

#include "engine/bgfx_utils.h"
#include "engine/particles.h"
#include "engine/asset_loader.h"

dsr::ParticleGenerator createParticleGenerator();

static dsr::Log _log{"main"};

using namespace std;
using namespace dsr;

int main() {

    vector<GameObject> objects = loadObjects("data/scripts/objects.json");

    uint16_t width = 800;
    uint16_t height = 600;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *wnd = SDL_CreateWindow(
            "Particle Editor",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    bgfx::sdlSetWindow(wnd);
    bgfx::init();
    bgfx::reset(width, height, BGFX_RESET_VSYNC);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x313233ff, 1.0f, 0);

    dsr::ParticleGenerator generator = createParticleGenerator();

    dsr::ParticleSystem particleSystem;
    particleSystem.addGenerator(&generator);

    F32 view[16];
    bx::mtxIdentity(view);

    F32 proj[16];
    bx::mtxOrtho(proj, 0, width, height, 0, 1, 100);

    bgfx::setViewTransform(0, view, proj);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    F32 mouse[3] = {0, 0, 0};
    F32 emitterSpeed = 2.0;

    int viewType = 0;

    SDL_Event event;
    bool running = true;
    while (running) {

        // input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEMOTION) {
                mouse[0] = event.motion.x;
                mouse[1] = event.motion.y;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if (emitterSpeed == 2) {
                    emitterSpeed = 0;
                } else {
                    emitterSpeed = 2;
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
                viewType = 1;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
                viewType = 2;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_3) {
                viewType = 3;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_0) {
                viewType = 0;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                generator.generatorSpawnArc -= 0.3;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w) {
                generator.generatorSpawnArc += 0.3;
            }
        }

        F32 direction[3];
        bx::vec3Sub(direction, mouse, generator.generatorPosition);
        bx::vec3Norm(direction, direction);
        bx::vec3Mul(direction, direction, emitterSpeed);

        if (viewType == 1) {
            bx::vec3Move(generator.generatorSpawnDirection, direction);
        }

        if (viewType == 2) {
            bx::vec3Add(generator.generatorPosition, generator.generatorPosition, direction);
        }

        if (viewType == 3) {
            dsr::vec3Rot2(generator.generatorSpawnDirection, generator.generatorSpawnDirection, 0.2);
        }

        // update
        particleSystem.update(bgfx::TimeUnit::fromMilliseconds(33.3));

        bgfx::submit(0);

        // render
        particleSystem.render();

        bgfx::submit(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(1, 2, 0x0B, "FPS: unknown");

        bgfx::frame();
    }

    return 0;
}

dsr::ParticleGenerator createParticleGenerator() {
    dsr::ParticleGenerator generator;
    generator.generatorSpawnFrequency = 32;

    generator.generatorPosition[0] = 300;
    generator.generatorPosition[1] = 300;
    generator.generatorPosition[2] = 0;

    generator.generatorSpawnRadius = 2.0;

    generator.generatorSpawnArc = 3.14;

    generator.generatorSpawnDirection[0] = 1.0f;
    generator.generatorSpawnDirection[1] = 0.0f;

    generator.params.lifetime = {bgfx::TimeUnit::fromSeconds(3), bgfx::TimeUnit::fromSeconds(5)};

    generator.params.startScale = {1, 2};
    generator.params.endScale = {6, 10};

    generator.params.startSpeed = {2, 4};
    generator.params.endSpeed = {0.1, 0.6};

    generator.params.startRotation = {0, 0};
    generator.params.endRotation = {4, 6};

    generator.params.startAlpha = {1, 1};
    generator.params.endAlpha = {0, 0};

    generator.params.startColor = {0x324050ff, 0x324050ff};
    generator.params.endColor = {0x600090ff, 0x604090ff};
    return generator;
};
