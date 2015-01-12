#include "BoneArmature2D.h"

#include "Bone2D.h"

engine::BoneArmature2D::BoneArmature2D() {
}

engine::BoneArmature2D::~BoneArmature2D() {
}

engine::Bone2D *engine::BoneArmature2D::getRootBone() const {
    return rootNode;
}

engine::Bone2D *engine::BoneArmature2D::createRootBone(Identity id) {
    if (rootNode == nullptr) {
        rootNode = new Bone2D{this, id};
    }

    return rootNode;
}
