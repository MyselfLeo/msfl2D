//
// Created by leo on 5/19/23.
//

#include <cmath>

#include "Vec2D.hpp"



namespace Msfl2D {
    const Vec2D Vec2D::ZERO = {0, 0};


    inline double Vec2D::norm() const {
        return sqrt(x*x + y*y);
    }

    Vec2D Vec2D::normalized() const {
        return *this / norm();
    }


    double Vec2D::dot(const Vec2D &v1, const Vec2D &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    double Vec2D::cross(const Msfl2D::Vec2D &v1, const Msfl2D::Vec2D &v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    Vec2D Vec2D::cross(const Vec2D &v, double s) {
        return {s * v.y, -s*v.x};
    }

    Vec2D Vec2D::cross(double s, const Vec2D &v) {
        return {-s * v.y, s*v.x};
    }





    Vec2D Vec2D::operator+(const Vec2D &other) const {
        return {x + other.x, y + other.y};
    }

    Vec2D& Vec2D::operator+=(const Vec2D &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2D Vec2D::operator-(const Vec2D &other) const {
        return {x - other.x, y - other.y};
    }

    Vec2D& Vec2D::operator-=(const Vec2D &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2D Vec2D::operator*(double s) const {
        return {x*s, y*s};
    }

    Vec2D& Vec2D::operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2D Vec2D::operator/(double s) const {
        return {x/s, y/s};
    }

    Vec2D& Vec2D::operator/=(double s) {
        x /= s;
        y /= s;
        return *this;
    }


    std::ostream &operator<<(std::ostream &os, const Vec2D &v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    double Vec2D::distance_squared(const Vec2D &v1, const Vec2D &v2) {
        return pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2);
    }

    double Vec2D::distance(const Vec2D &v1, const Vec2D &v2) {
        return sqrt(Vec2D::distance_squared(v1, v2));
    }

    Vec2D Vec2D::operator-() const {
        return {-x, -y};
    }
} // Msfl2D




