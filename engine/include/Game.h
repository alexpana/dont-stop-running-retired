#pragma once

#include <memory>
#include <string>

#include <SDL.h>

#include "Filesystem.h"
#include "graphics/Renderer.h"
#include "Random.h"
#include "TextureFactory.h"
#include "Timer.h"

namespace engine {
    class Game;

    typedef std::shared_ptr<Game> GamePtr;

    class Game {
    public:
        class Params {
        public:
            int screenWidth;
            int screenHeight;
            std::string windowTitle;
        };

        class IUpdateable {
        public:
            virtual void update(double timeDelta) = 0;

            virtual ~IUpdateable() {
            };
        };

        typedef std::shared_ptr<IUpdateable> IUpdateablePtr;

        class IDrawable {
        public:
            virtual void draw() = 0;

            virtual ~IDrawable() {
            };
        };

        typedef std::shared_ptr<IDrawable> IDrawablePtr;

    public:
        Game();

        ~Game();

        Game(const Game &other) = delete;

        bool initialize(const Params &params);

        SDL_Window *getWindow() const {
            return m_mainWindow;
        }

        int getScreenHeight() const {
            return m_frameBuffer->h;
        }

        int getScreenWidth() const {
            return m_frameBuffer->w;
        }

        bool isInitialized() const {
            return m_initialized;
        }

        Filesystem &getFilesystem() {
            return m_filesystem;
        }

        graphics::RendererPtr getRenderer() {
            return m_renderer;
        }

        TextureFactory &getTextureFactory() {
            return *m_imageFactory;
        }

        Random &getRandom() {
            return m_random;
        }

        int getFrameCount() const {
            return m_frameCount;
        }

        void startFrame();

        void endFrame();

        void update();

        bool isRunning();

        void stopRunning();

        void registerUpdateable(const IUpdateablePtr &updateable);

        void registerDrawable(const IDrawablePtr &drawable);

    private:
        bool initSDL();

        bool initSDLImage();

        bool initSDLTTF();

        SDL_Window *m_mainWindow;

        SDL_Surface *m_frameBuffer;

        Random m_random;

        Filesystem m_filesystem;

        graphics::RendererPtr m_renderer;

        TextureFactoryPtr m_imageFactory;

        Timer m_timer;

        bool m_initialized;

        bool m_isRunning;

        int m_frameCount;

        double m_lastFrameTimeDelta;

        std::vector<IUpdateablePtr> m_registeredUpdateables;

        std::vector<IDrawablePtr> m_registeredDrawables;
    };

}