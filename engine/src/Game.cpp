#include "Game.h"

#include <SDL_ttf.h>

#include "sdl/SDLInput.h"
#include "Updateable.h"
#include "Renderable.h"

namespace engine {

    Game::Game(Params const &params) : initParams(params), logger("Game") {
    }

    Game::~Game() {
        //Destroy window
        SDL_DestroyWindow(mainWindow);
        mainWindow = nullptr;
    }

    bool Game::start() {
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
            logger.error() << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        auto nativeRenderer = SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        frameBuffer = SDL_GetWindowSurface(mainWindow);

        renderer = std::unique_ptr<Renderer>(new Renderer{nativeRenderer});

        imageFactory = std::unique_ptr<TextureFactory>(new TextureFactory{nativeRenderer, *frameBuffer->format});

        random = std::unique_ptr<Random>(new Random);

        input = std::unique_ptr<Input>(new SDLInput);

        initialized = true;

        gameTimer.restart();

        return true;
    }

    void Game::stop() {
        if (initialized) {
            sound->cleanup();

            stopSDLTTF();
            stopSDLImage();
            stopSDL();

            initialized = false;
        }
    }

    bool Game::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
            logger.error() << "Could not init SDL_VIDEO" << std::endl;
            return false;
        }

        return true;
    }

    bool Game::initSDLImage() {
        int flags = IMG_INIT_PNG;

        int status = IMG_Init(flags);
        if ((status & flags) != flags) {
            logger.error() << "IMG_Init: Failed to init required png support!" << std::endl;
            logger.error() << "IMG_Init: " << IMG_GetError() << std::endl;
            return false;
        }

        return true;
    }

    bool Game::initSDLTTF() {
        return TTF_Init() == 0;
    }

    void Game::registerUpdateable(Updateable *updateable) {
        registeredUpdateables.push_back(updateable);
    }

    void Game::registerRenderable(Renderable *drawable) {
        registeredRenderables.push_back(drawable);
    }

    void Game::startFrame() {
        frameTimer.restart();
    }

    void Game::endFrame() {
        frameCount += 1;
        renderer->flip();

        lastFrameTimeDelta = frameTimer.seconds();
    }

    void Game::update() {
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

    void Game::stopSDL() {
        SDL_Quit();
    }

    void Game::stopSDLImage() {
        IMG_Quit();
    }

    void Game::stopSDLTTF() {
        TTF_Quit();
    }
}