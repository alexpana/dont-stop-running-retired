#pragma once

#include <BoneArmature2D.h>
#include "TestingFramework.h"

#include "Bone2D.h"
#include "MathUtils.h"

class TestBones : public test::Test {

    BEGIN_TEST(boneHierarchySimple)
        using namespace engine;

        BoneArmature2D armature;

        Bone2D *root = armature.createRootBone(Identity::generateRandom());

        Bone2D *c1 = root->createChild(Identity::generateRandom(), 2.0, Degrees{90});

        Bone2D *c2 = root->createChild(Identity::generateRandom(), 5.0, Degrees{180});

        Bone2D *c21 = c2->createChild(Identity::generateRandom(), 2.0, Degrees{90});

        ASSERT_EQUALS(c1->getPosition(), (Vec2{0.0, 2.0}));

        ASSERT_EQUALS(c2->getPosition(), (Vec2{-5.0, 0.0}));

        ASSERT_EQUALS(c21->getPosition(), (Vec2{-5.0, 2.0}));

        ASSERT_EQUALS(*(root->childrenBegin()), c1);

        ASSERT_EQUALS(*(root->childrenBegin() + 1), c2);

        ASSERT_EQUALS(root->getArmature(), &armature);
        ASSERT_EQUALS(c1->getArmature(), &armature);
        ASSERT_EQUALS(c2->getArmature(), &armature);
        ASSERT_EQUALS(c21->getArmature(), &armature);
    END_TEST

    void registerTests() override {
        REGISTER_TEST(boneHierarchySimple);
    }
};