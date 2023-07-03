//
// Created by myselfleo on 14/06/23.
//

#include <cmath>
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
        if (is_vertical()) {throw GeometryException("Infinite number of solutions for y = ? in a vertical line.");}
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

    Vec2D Line::intersection(const Line &l1, const Line &l2) {
        Vec2D vec_1 = l1.get_vec();
        Vec2D vec_2 = l2.get_vec();
        Vec2D p1 = l1.get_origin();
        Vec2D p2 = l2.get_origin();

        // collinear lines never intersect
        if (Vec2D::collinear(vec_1, vec_2)) {throw GeometryException("Collinear lines cannot intersect.");}

        // from https://www.av8n.com/physics/points-lines.htm#sec-derivation
        Vec2D n2 = vec_2.rotate(M_PI/2);
        return p1 + vec_1 * Vec2D::dot((p2 - p1), n2) / (Vec2D::dot(n2, vec_1));
    }

    bool Line::overlap(const Line &l1, const Line &l2) {
        // If the 2 lines are not collinear, return false
        if (!Vec2D::collinear(l1.get_vec(), l2.get_vec())) {return false;}

        // the origin of l2 must be on l1. We could do the inverse too.
        return l1.has(l2.get_origin());
    }

    bool Line::has(const Vec2D &p) const {
        if (is_vertical()) {return find_x(p.y) == p.x;}
        else {return find_y(p.x) == p.y;}
    }

    double Line::get_grad_coo(const Vec2D &p) const {
        // we need to find n such as v1 = v2.n
        // where v2 is the dir vec of the line (normalised, so its length is 1 graduation)
        // & v1 is the vector from the line origin to the given point.
        Vec2D v1 = p - get_origin();
        Vec2D v2 = get_vec();

        // If ratio_x != ratio_y, the given point is not on the line.
        double ratio_x = v1.x / v2.x;
        double ratio_y = v1.y / v2.y;

        if (ratio_x != ratio_y) {throw GeometryException("The point given to get_grad_coo() is not on the line.");}
        return ratio_x;
    }
} // Msfl2D