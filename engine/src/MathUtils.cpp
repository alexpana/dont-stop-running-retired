#include "MathUtils.h"

namespace engine {
    Angle::Angle(const Radians &radians) : value(radians) {
    }

    Angle::Angle(const Degrees &degrees) : value(degrees) {
    }

    Radians Angle::getRadians() {
        return value;
    }

    Degrees Angle::getDegrees() {
        return Degrees(value);
    }

    Angle::operator double() const {
        return value;
    }

    Radians::Radians(double value) : value(value) {
    }

    Radians::Radians(Degrees degrees) : value(degrees.value * 0.01745329251) {
    }

    Radians::operator double() const {
        return value;
    }

    Degrees::Degrees(double value) : value(value) {
    }

    Degrees::Degrees(Radians radians) : value(value * 57.2957795131) {
    }

    Degrees::operator double() const {
        return value;
    }
}