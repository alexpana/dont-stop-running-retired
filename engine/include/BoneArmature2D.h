#pragma once

#include <memory>
#include <vector>
#include "Identity.h"

namespace engine {
    class Bone2D;

    // The armature owns all the bones it contains,
    // and is responsible for properly releasing them
    class BoneArmature2D {
    public:
        BoneArmature2D();

        ~BoneArmature2D();

        Bone2D *createRootBone(Identity id);

        Bone2D *getRootBone() const;

//        void addBone(std::unique_ptr<Bone2D> &&bone);

    private:
        Bone2D *rootNode = nullptr;
        std::vector<std::unique_ptr<Bone2D>> bones;
    };
}