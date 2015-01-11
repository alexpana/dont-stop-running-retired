#pragma once

#include "TestingFramework.h"
#include "MathUtils.h"

class TestMath : public test::Test {

    BEGIN_TEST(degreesToRadiansConversion)
        using namespace engine;

        Degrees degrees(180);
        Radians radians(degrees);

        ASSERT_FLOAT_EQUALS(radians.value, Constants::PI);
    END_TEST

    BEGIN_TEST(radiansToDegreesConversion)
        using namespace engine;

        Radians radians(Constants::PI / 2.0);
        Degrees degrees(radians);

        ASSERT_FLOAT_EQUALS(degrees.value, 90.0);
    END_TEST

    BEGIN_TEST(radiansToAnglesConversion)
        using namespace engine;

        Radians radians(Constants::PI / 2.0);
        Angle angle{radians};

        ASSERT_EQUALS(angle.getRadians(), radians);
    END_TEST

    BEGIN_TEST(degreesToAnglesConversion)
        using namespace engine;

        Degrees degrees(180);
        Angle angle{degrees};

        ASSERT_EQUALS(angle.getDegrees(), degrees);
    END_TEST

    void registerTests() override {
        REGISTER_TEST(degreesToRadiansConversion);
        REGISTER_TEST(radiansToDegreesConversion);
        REGISTER_TEST(radiansToAnglesConversion);
        REGISTER_TEST(degreesToAnglesConversion);
    }
};