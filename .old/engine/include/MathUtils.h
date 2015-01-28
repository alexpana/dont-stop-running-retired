#pragma once

#include <cmath>

namespace engine {
    struct Constants {
        static constexpr double PI = 3.14159265359;
    };

    struct Degrees;

    struct Radians {
        Radians(double value);

        Radians(Degrees degrees);

        operator double() const;

        double value;
    };

    struct Degrees {
        explicit Degrees(double value);

        Degrees(Radians radians);

        operator double() const;

        double value;
    };

    struct Angle {
        Angle(const Radians &radians);

        explicit Angle(const Degrees &degrees);

        Radians getRadians();

        Degrees getDegrees();

        operator double() const;

        /**
        * Angle stored in Radians.
        */
        Radians value;
    };

    bool operator!=(const Degrees& lhs, const Degrees& rhs);

    bool operator!=(const Radians& lhs, const Radians& rhs);

    bool operator!=(const Angle& lhs, const Angle& rhs);

    bool operator==(const Degrees& lhs, const Degrees& rhs);

    bool operator==(const Radians& lhs, const Radians& rhs);

    bool operator==(const Angle& lhs, const Angle& rhs);
}