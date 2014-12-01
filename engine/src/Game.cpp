#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"

namespace engine {

    Game::Game() :
            m_initialized(false),
            m_isRunning(false),
            m_frameCount(0),
            m_lastFrameTimeDelta(0) {
    }

    bool Game::initialize(Params const &params) {
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

        //Create window
        m_mainWindow = SDL_CreateWindow(params.windowTitle.c_str(), 1000, SDL_WINDOWPOS_UNDEFINED, params.screenWidth, params.screenHeight, SDL_WINDOW_SHOWN);

        if (m_mainWindow == nullptr) {
            printf("Window could not be created! SDL_Error: %s", SDL_GetError());
            return false;
        }

        auto nativeRenderer = SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //Get window surface
        m_frameBuffer = SDL_GetWindowSurface(m_mainWindow);

        m_renderer = graphics::Renderer::create(nativeRenderer);

        m_imageFactory = TextureFactory::create(nativeRenderer, *m_frameBuffer->format);

        m_initialized = true;

        m_isRunning = true;

        return true;
    }

    bool Game::initSDLImage() {
        int flags = IMG_INIT_JPG | IMG_INIT_PNG;

        int status = IMG_Init(flags);
        if ((status & flags) != flags) {
            printf("IMG_Init: Failed to init required jpg and png support!\n");
            printf("IMG_Init: %s\n", IMG_GetError());
            return false;
        }

        return true;
    }

    bool Game::initSDLTTF() {
        return TTF_Init() == 0;
    }

    bool Game::initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("Could not initialize SDL_VIDEO");
            return false;
        }

        return true;
    }

    Game::~Game() {
        //Destroy window
        SDL_DestroyWindow(m_mainWindow);
        m_mainWindow = nullptr;

        //Quit SDL subsystems
        SDL_Quit();
    }

    void Game::registerUpdateable(const IUpdateablePtr &updateable) {
        m_registeredUpdateables.push_back(updateable);
    }

    void Game::registerDrawable(const IDrawablePtr &drawable) {
        m_registeredDrawables.push_back(drawable);
    }

    void Game::startFrame() {
        m_timer.restart();
    }

    void Game::endFrame() {
        m_frameCount += 1;
        m_renderer->flip();

        m_lastFrameTimeDelta = 10; //m_timer.getMilli();
    }

    void Game::update() {
        m_lastFrameTimeDelta = 15;
        for (auto &updateable : m_registeredUpdateables) {
            updateable->update(m_lastFrameTimeDelta);
        }

        // Draw
        m_renderer->setColor(0x303132ff);
        m_renderer->clear();

        for (auto &drawable : m_registeredDrawables) {
            drawable->draw();
        }
    }

    bool Game::isRunning() {
        return m_isRunning;
    }

    void Game::stopRunning() {
        m_isRunning = false;
    }
}