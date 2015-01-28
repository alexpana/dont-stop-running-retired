#pragma once

namespace engine {

    class Renderer;

    /**
    * Interface for a component that can be drawn by a renderer.
    */
    class Renderable {
    public:
        virtual void render(Renderer *renderer) = 0;

        virtual ~Renderable() {
        };
    };
}