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
    std::string windowName = "Don't Stop Running";

    dsr::initBgfx(width, height, windowName);

    dsr::ParticleGenerator generator = createParticleGenerator();

    dsr::ParticleSystem particleSystem;
    particleSystem.addGenerator(&generator);

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
        particleSystem.update(dsr::TimeUnit::fromMilliseconds(33.3));

        // render
        particleSystem.render();

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

    generator.params.lifetime = {dsr::TimeUnit::fromSeconds(3), dsr::TimeUnit::fromSeconds(5)};

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
