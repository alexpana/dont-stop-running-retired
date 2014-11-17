#pragma once

#include <memory>
#include <string>

#include <SDL.h>

#include "Filesystem.h"
#include "graphics/Renderer.h"
#include "TextureFactory.h"
#include "physics/Physics.h"
#include "Random.h"

namespace engine {

class Game;
typedef std::shared_ptr<Game> GamePtr;

class Game 
{
public:
    class Params {
    public:
        int screenWidth;
        int screenHeight;
        std::string windowTitle;
    };

public:
    Game();

    ~Game();

    Game(const Game& other) = delete;

    bool initialize(const Params& params);

    SDL_Window* Game::getWindow() const 
    {
        return m_mainWindow;
    }

    int getScreenHeight() const 
    {
        return m_frameBuffer->h;
    }

    int getScreenWidth() const 
    {
        return m_frameBuffer->w;
    }

    bool isInitialized() const 
    {
        return m_initialized;
    }

    Filesystem& getFilesystem() 
    {
        return m_filesystem;
    }

    physics::Physics& getPhysics()
    {
        return m_physics;
    }

    graphics::RendererPtr getRenderer() 
    {
        return m_renderer;
    }

    TextureFactory& getTextureFactory() 
    {
        return *m_imageFactory;
    }

    Random& getRandom()
    {
        return m_random;
    }

    int getFrameCount() const
    {
        return m_frameCount;
    }

    void startFrame();

    void endFrame();

    bool isRunning();

    void stopRunning();

private:
    bool initSDL();

    bool initSDLImage();

    SDL_Window *m_mainWindow;

    SDL_Surface *m_frameBuffer;

    Random m_random;

    Filesystem m_filesystem;

    physics::Physics m_physics;

    graphics::RendererPtr m_renderer;

    TextureFactoryPtr m_imageFactory;

    bool m_initialized;

    bool m_isRunning;

    int m_frameCount;
};

}