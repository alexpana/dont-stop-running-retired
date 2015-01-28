#pragma once

#include "MathUtils.h"
#include "Vec2.h"
#include "Identity.h"
#include "BoneArmature2D.h"

namespace engine {
    class BoneArmature2D;

    class Bone2D {
    public:
        Bone2D(BoneArmature2D* armature, const Identity id, double length = 0.0, Radians rotation = 0.0);

        Bone2D *getParent();

        Bone2D *getRootParent();

        BoneArmature2D *getArmature() const;

        double getLength() const;

        void setLength(double length);

        Radians getRotation() const;

        void setRotation(Radians rotation);

        Vec2 getPosition() const;

        Identity getIdentity() const;

        Bone2D *createChild(const Identity id, double length = 0.0, Radians rotation = 0.0);

        std::vector<Bone2D*>::iterator childrenBegin();

        std::vector<Bone2D*>::iterator childrenEnd();

    private:
        Bone2D(Bone2D *parent, const Identity id, double length = 0.0, Radians rotation = 0.0);

        BoneArmature2D *armature;

        Identity identity;

        Bone2D *parent = nullptr;

        double length = 0;

        Radians rotation = 0;

        std::vector<Bone2D *> children;
    };
}