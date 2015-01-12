#include "Bone2D.h"

namespace engine {
    Bone2D::Bone2D(BoneArmature2D *armature, const Identity id, double length, Radians rotation) :
            armature(armature),
            identity{id},
            parent{nullptr},
            length(length),
            rotation(rotation) {
    }

    Bone2D *Bone2D::getParent() {
        return parent;
    }

    double Bone2D::getLength() const {
        return length;
    }

    Radians Bone2D::getRotation() const {
        return rotation;
    }

    Identity Bone2D::getIdentity() const {
        return identity;
    }

    Vec2 Bone2D::getPosition() const {
        Vec2 parentPosition{0, 0};
        if (parent != nullptr) {
            parentPosition = parent->getPosition();
        }

        return parentPosition + Vec2::angleDirection(rotation) * length;
    }

    Bone2D *Bone2D::getRootParent() {
        Bone2D *root = this;
        while (root->parent != nullptr) {
            root = root->parent;
        }

        return root;
    }

    void Bone2D::setLength(double length) {
        this->length = length;
    }

    void Bone2D::setRotation(Radians rotation) {
        this->rotation = rotation;
    }

    Bone2D *Bone2D::createChild(const Identity id, double length, Radians rotation) {
        Bone2D *child = new Bone2D{this, id, length, rotation};
        children.push_back(child);
        return child;
    }

    Bone2D::Bone2D(Bone2D *parent, const Identity id, double length, Radians rotation) :
            identity{id},
            parent{parent},
            length{length},
            rotation{rotation} {
        armature = parent->armature;
    }

    std::vector<Bone2D *>::iterator Bone2D::childrenBegin() {
        return children.begin();
    }

    std::vector<Bone2D *>::iterator Bone2D::childrenEnd() {
        return children.end();
    }

    BoneArmature2D *Bone2D::getArmature() const {
        return armature;
    }
};