//
// Created by myselfleo on 14/06/23.
//

#include "Line.hpp"
#include "MsflExceptions.hpp"

namespace Msfl2D {
    Line::Line(const Vec2D &p1, const Vec2D &p2): p1(p1), p2(p2) {
        if (p1 == p2) {throw GeometryException("The 2 given points are equal.");}
    }


    Line Line::from_director_vector(const Vec2D &p, const Vec2D &dir_vec) {
        return Line(p, p + dir_vec);
    }



    LineSide Line::side(const Vec2D &p) const {
        return Line::side(p1, p2, p);
    }


    LineSide Line::side(const Vec2D &p1, const Vec2D &p2, const Vec2D &p) {
        // cross product of (p2-p1, p-p1)
        double orient = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
        if (orient > 0) {return LEFT;}
        if (orient < 0) {return RIGHT;}
        return MIDDLE;
    }

    double Line::find_y(double x) const {
        if (p1.x == p2.x) {throw GeometryException("Infinite number of solutions for y = ? in a vertical line.");}
        return get_slope() * x + get_zero();
    }

    double Line::find_x(double y) const {
        if (is_vertical()) {return p1.x;}
        if (p1.y == p2.y) {throw GeometryException("Infinite number of solutions for x = ? in an horizontal line.");}
        return (y - get_zero()) / get_slope();
    }

    bool Line::is_vertical() const {
        return p1.x == p2.x;
    }

    Vec2D Line::get_vec() const {
        return (p2 - p1).normalized();
    }

    double Line::get_slope() const {
        if (is_vertical()) {throw GeometryException("No slope for a vertical line.");}
        return (p2.y - p1.y) / (p2.x - p1.x);
    }

    double Line::get_zero() const {
        return p1.y - get_slope() * p1.x;
    }

    Vec2D Line::get_origin() const {
        return p1;
    }

    Vec2D Line::get_coo_grad(double g) const {
        return get_origin() + get_vec() * g;
    }

    std::ostream &operator<<(std::ostream &os, const Line &l) {
        os << l.p1 << " -> " << l.p2;
        return os;
    }
} // Msfl2D