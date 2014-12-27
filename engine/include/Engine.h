#pragma once

#include <string>

#include <SDL.h>

#include "Filesystem.h"
#include "InputSystem.h"
#include "Memory.h"
#include "Renderer.h"
#include "Random.h"
#include "Sound.h"
#include "TextureFactory.h"
#include "Timer.h"

namespace engine {
    class Engine;

    class Updateable;

    class Renderable;

    class Engine {
    public:
        struct Params {
            int screenWidth;
            int screenHeight;
            std::string windowTitle;
        };

    public:
        Engine(const Params &params);

        Engine(const Engine &other) = delete;

        ~Engine();

        bool start();

        void stop();

        SDL_Window *getWindow() const {
            return mainWindow;
        }

        int getScreenHeight() const {
            return frameBuffer->h;
        }

        int getScreenWidth() const {
            return frameBuffer->w;
        }

        bool isInitialized() const {
            return initialized;
        }

        Filesystem *getFilesystem() {
            return filesystem.get();
        }

        Renderer *getRenderer() {
            return renderer.get();
        }

        TextureFactory *getTextureFactory() {
            return imageFactory.get();
        }

        Random *getRandom() {
            return random.get();
        }

        Sound *getSound() {
            return sound.get();
        }

        InputSystem *getInput() {
            return input.get();
        }

        int getFrameCount() const {
            return frameCount;
        }

        /**
        * Returns the seconds elapsed since starting the engine with Engine::start()
        */
        double getElapsedTime() const {
            return gameTimer.seconds();
        }

        void startFrame();

        void endFrame();

        void update();

        void registerUpdateable(Updateable *updateable);

        void registerRenderable(Renderable *renderable);

    private:
        bool initSDL();

        void stopSDL();

        bool initSDLImage();

        void stopSDLImage();

        bool initSDLTTF();

        void stopSDLTTF();

        SDL_Window *mainWindow;

        SDL_Surface *frameBuffer;

        std::unique_ptr<Random> random;

        std::unique_ptr<Renderer> renderer;

        std::unique_ptr<Sound> sound;

        std::unique_ptr<Filesystem> filesystem;

        std::unique_ptr<TextureFactory> imageFactory;

        std::unique_ptr<InputSystem> input;

        bool initialized = false;

        int frameCount = 0;

        double lastFrameTimeDelta = 0;

        std::vector<Updateable *> registeredUpdateables;

        std::vector<Renderable *> registeredRenderables;

        Timer frameTimer;

        Timer gameTimer;

        const Params initParams;
    };
}