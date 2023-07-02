//
// Created by myselfleo on 5/19/23.
//

#include <cmath>

#include "Vec2D.hpp"
#include "Line.hpp"


namespace Msfl2D {
    const Vec2D Vec2D::ZERO = {0, 0};

    bool Vec2D::operator==(const Vec2D &other) const {
        return x == other.x && y == other.y;
    }

    bool Vec2D::operator!=(const Msfl2D::Vec2D &other) const {
        return x != other.x || y != other.y;
    }

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

    double Vec2D::det(const Vec2D &v1, const Vec2D &v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    double Vec2D::project(const Line &line) const {
        Vec2D line_vec_dir = line.get_vec();
        Vec2D point_vec_dir = *this - line.get_origin();
        return Vec2D::dot(point_vec_dir, line_vec_dir) / Vec2D::dot(line_vec_dir, line_vec_dir);
    }

    Vec2D Vec2D::rotate(double angle) const {
        return {x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle)};
    }

    Vec2D Vec2D::rotate(double angle, const Vec2D &center) const {
        Vec2D res = *this - center;
        res = res.rotate(angle);
        return res + center;
    }

    bool Vec2D::collinear(const Vec2D &v1, const Vec2D &v2) {
        return cross(v1, v2) == 0;
    }
} // Msfl2D




