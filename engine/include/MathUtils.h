#pragma once

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
}