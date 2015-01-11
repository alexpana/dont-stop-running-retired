#include "Bone2D.h"

namespace engine {
    Bone2D::Bone2D(const Bone2D *parent, double length, Radians rotation) :
            parent{parent},
            length(length),
            rotation(rotation) {
    }

    const Bone2D *Bone2D::getParent() const {
        return parent;
    }

    double Bone2D::getLength() const {
        return length;
    }

    Radians Bone2D::getRotation() const {
        return rotation;
    }

    Vec2 Bone2D::getPosition() const {
        Vec2 parentPosition{0, 0};
        if (parent != nullptr) {
            parentPosition = parent->getPosition();
        }

        return parentPosition + Vec2::angleDirection(rotation) * length;
    }

    void Bone2D::setLength(double length) {
        this->length = length;
    }

    void Bone2D::setRotation(Radians rotation) {
        this->rotation = rotation;
    }
};