#pragma once

namespace engine {

    /**
    * Interface for a component that can be updated.
    */
    class Updateable {
    public:
        virtual void update(double timeDelta) = 0;

        virtual ~Updateable() {
        };
    };
}
