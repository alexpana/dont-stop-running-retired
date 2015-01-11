#pragma once

#include "TestingFramework.h"

#include "Bone2D.h"
#include "MathUtils.h"

class TestBones : public test::Test {

    BEGIN_TEST(boneHierarchySimple)
        using namespace engine;

        Bone2D parent{nullptr};
        auto b1 = Bone2D{&parent, 2.0, Degrees{90}};

        ASSERT_EQUALS(b1.getPosition(), (Vec2{0.0, 2.0}));

    END_TEST

    void registerTests() override {
        REGISTER_TEST(boneHierarchySimple);
    }
};