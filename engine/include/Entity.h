#pragma once

#include <memory>
#include <unordered_map>

#include "Updateable.h"
#include "Transform.h"

namespace engine {
    class Renderable;

    class Transform;

    class Entity : public Updateable {
    public:
        Renderable *getRenderComponent() {
            return renderComponent.get();
        };

        void setRenderComponent(std::unique_ptr<Renderable>&& renderComponent) {
            std::swap(this->renderComponent, renderComponent);
        }

        Transform &getTransformComponent() {
            return transform;
        }

        virtual void update(double timeDelta) = 0;

    private:
        Transform transform;
        std::unique_ptr<Renderable> renderComponent;
    };
}
