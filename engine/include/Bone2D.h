#pragma once

#include "MathUtils.h"
#include "Vec2.h"

namespace engine {
    class Bone2D {
    public:
        Bone2D(const Bone2D *parent, double length = 0.0, Radians rotation = 0.0);

        const Bone2D *getParent() const;

        double getLength() const;

        void setLength(double length);

        Radians getRotation() const;

        void setRotation(Radians rotation);

        Vec2 getPosition() const;

    private:
        const Bone2D *parent;

        double length;

        Radians rotation;
    };
}