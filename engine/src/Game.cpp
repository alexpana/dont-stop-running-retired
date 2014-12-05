#include "Game.h"

#include <SDL_ttf.h>

#include "Sound.h"

namespace engine {

    Game::Game(Params const &params) : initParams(params) {
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
            printf("Window could not be created! SDL_Error: %s", SDL_GetError());
            return false;
        }

        auto nativeRenderer = SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        frameBuffer = SDL_GetWindowSurface(mainWindow);

        renderer = std::unique_ptr<Renderer>(new Renderer{nativeRenderer});

        imageFactory = std::unique_ptr<TextureFactory>(new TextureFactory{nativeRenderer, *frameBuffer->format});

        random = std::unique_ptr<Random>(new Random);

        initialized = true;

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
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("Could not init SDL_VIDEO");
            return false;
        }

        return true;
    }

    bool Game::initSDLImage() {
        int flags = IMG_INIT_PNG;

        int status = IMG_Init(flags);
        if ((status & flags) != flags) {
            printf("IMG_Init: Failed to init required png support!\n");
            printf("IMG_Init: %s\n", IMG_GetError());
            return false;
        }

        return true;
    }

    bool Game::initSDLTTF() {
        return TTF_Init() == 0;
    }

    void Game::registerUpdateable(const IUpdateablePtr &updateable) {
        registeredUpdateables.push_back(updateable);
    }

    void Game::registerDrawable(const IDrawablePtr &drawable) {
        registeredDrawables.push_back(drawable);
    }

    void Game::startFrame() {
        timer.restart();
    }

    void Game::endFrame() {
        frameCount += 1;
        renderer->flip();

        lastFrameTimeDelta = 10; //m_timer.getMilli();
    }

    void Game::update() {
        lastFrameTimeDelta = 15;
        for (auto &updateable : registeredUpdateables) {
            updateable->update(lastFrameTimeDelta);
        }

        // Draw
        renderer->setColor(0x303132ff);
        renderer->clear();

        for (auto &drawable : registeredDrawables) {
            drawable->draw();
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