#pragma once

#include <cmath>
#include <ostream>

#include "MathUtils.h"

namespace engine {
    class Vec2 {
    public:
        union {
            double x;
            double w;
        };

        union {
            double y;
            double h;
        };

        Vec2() : x(0), y(0) {
        }

        Vec2(double x, double y) : x(x), y(y) {
        }

        Vec2 &operator+=(const Vec2 &rhs);

        Vec2 &operator*=(double rhs);

        Vec2 &operator-=(const Vec2 &rhs);

        double magnitude() const;

        double squaredMagnitude() const;

        double cross(const Vec2 &rhs) const;

        Vec2 normalized() const;

        static Vec2 angleDirection(Angle angle) {
            return Vec2{std::cos(angle.getRadians()), std::sin(angle.getRadians())};
        }
    };

    inline std::ostream &operator<<(std::ostream &lhs, const Vec2 &rhs) {
        return lhs << "Vec2{" << rhs.x << ", " << rhs.y << "}";
    }

    inline Vec2 &Vec2::operator+=(const Vec2 &rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    inline Vec2 &Vec2::operator*=(double rhs) {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }


    inline Vec2 &Vec2::operator-=(const Vec2 &rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }


    inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
        return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) {
        return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    inline Vec2 operator*(const Vec2 &lhs, double rhs) {
        return Vec2(lhs.x * rhs, lhs.y * rhs);
    }

    inline Vec2 operator*(double lhs, const Vec2 &rhs) {
        return Vec2(lhs * rhs.x, lhs * rhs.y);
    }

    inline bool operator==(const Vec2 &lhs, const Vec2 &rhs) {
        return fabs(lhs.x - rhs.x) < 0.00001 && fabs(lhs.y - rhs.y) < 0.00001;
    }

    inline bool operator!=(const Vec2 &lhs, const Vec2 &rhs) {
        return !(lhs == rhs);
    }

    inline double Vec2::cross(const Vec2 &rhs) const {
        return x * rhs.y - y * rhs.x;
    }

    inline double Vec2::magnitude() const {
        return sqrt(squaredMagnitude());
    }

    inline double Vec2::squaredMagnitude() const {
        return x * x + y * y;
    }

    inline Vec2 Vec2::normalized() const {
        return Vec2(*this) * (1.0 / magnitude());
    }
}