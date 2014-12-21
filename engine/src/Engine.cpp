#include "Engine.h"

#include <SDL_ttf.h>

#include "sdl/SDLInput.h"
#include "sdl/SDLTextureFactory.h"
#include "Updateable.h"
#include "Renderable.h"

static engine::Log _log{"Engine"};

namespace engine {

    Engine::Engine(Params const &params) : initParams(params) {
    }

    Engine::~Engine() {
        //Destroy window
        SDL_DestroyWindow(mainWindow);
        mainWindow = nullptr;
    }

    bool Engine::start() {
        using namespace std;

        if (!initSDL()) {
            return false;
        }

        if (!initSDLImage()) {
            return false;
        }

        if (!initSDLTTF()) {
            return false;
        }

        sound = std::unique_ptr<Sound>(new Sound);
        sound->initialize();

        //Create window
        mainWindow = SDL_CreateWindow(initParams.windowTitle.c_str(), 1000, SDL_WINDOWPOS_UNDEFINED, initParams.screenWidth, initParams.screenHeight, SDL_WINDOW_SHOWN);

        if (mainWindow == nullptr) {
            _log.error() << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        auto nativeRenderer = SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        frameBuffer = SDL_GetWindowSurface(mainWindow);

        renderer = std::unique_ptr<Renderer>(new Renderer{nativeRenderer});

        imageFactory = std::unique_ptr<TextureFactory>(new SDLTextureFactory{renderer.get()});

        random = std::unique_ptr<Random>(new Random);

        input = std::unique_ptr<Input>(new SDLInput);

        initialized = true;

        gameTimer.restart();

        return true;
    }

    void Engine::stop() {
        if (initialized) {
            sound->cleanup();

            stopSDLTTF();
            stopSDLImage();
            stopSDL();

            initialized = false;
        }
    }

    bool Engine::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
            _log.error() << "Could not init SDL_VIDEO" << std::endl;
            return false;
        }

        return true;
    }

    bool Engine::initSDLImage() {
        int flags = IMG_INIT_PNG;

        int status = IMG_Init(flags);
        if ((status & flags) != flags) {
            _log.error() << "IMG_Init: Failed to init required png support!" << std::endl;
            _log.error() << "IMG_Init: " << IMG_GetError() << std::endl;
            return false;
        }

        return true;
    }

    bool Engine::initSDLTTF() {
        return TTF_Init() == 0;
    }

    void Engine::registerUpdateable(Updateable *updateable) {
        registeredUpdateables.push_back(updateable);
    }

    void Engine::registerRenderable(Renderable *drawable) {
        registeredRenderables.push_back(drawable);
    }

    void Engine::startFrame() {
        frameTimer.restart();
    }

    void Engine::endFrame() {
        frameCount += 1;
        renderer->flip();

        lastFrameTimeDelta = frameTimer.seconds();
    }

    void Engine::update() {
//        lastFrameTimeDelta = 15;

        input->update();

        for (auto &updateable : registeredUpdateables) {
            updateable->update(lastFrameTimeDelta);
        }

        // Draw
        renderer->setColor(0x303132ff);
        renderer->clear();

        for (auto &drawable : registeredRenderables) {
            drawable->render(renderer.get());
        }
    }

    void Engine::stopSDL() {
        SDL_Quit();
    }

    void Engine::stopSDLImage() {
        IMG_Quit();
    }

    void Engine::stopSDLTTF() {
        TTF_Quit();
    }
}