//
// Created by myselfleo on 14/06/23.
//

#include "Line.hpp"
#include "GeometryException.hpp"

namespace Msfl2D {

    Line::Line(double a, double b): a(a), b(b), vertical(false) {}


    Line::Line(const Vec2D &p1, const Vec2D &p2) {
        if (p1 == p2) {throw GeometryException("The 2 given points are equal.");}

        if (p2.x == p1.x) {
            vertical = true;
            vert_x = p1.x;
            reversed = p2.y < p1.y;
        }
        else {
            vertical = false;
            a = (p2.y - p1.y) / (p2.x - p1.x);
            b = p1.y - a * p1.x;                // p1.y = a * p1.x + b   <=>   b = p1.y - a * p1.x
        }
    }



    LineSide Line::side(const Vec2D &p) const {
        if (vertical) {
            // we just have to check the x axis and the line orientation; no need for vector math
            if (p.x == vert_x) {return MIDDLE;}

            // With (p.x > x), true = right and false = left.
            // If the line is reversed, then we should have false = right and true = left.
            // this inversion is made using the XOR (^) operator.
            else if ((p.x > vert_x) ^ reversed) {return RIGHT;}
            else {return LEFT;}
        }
        else {
            double orient = Vec2D::cross(Vec2D(1, a), p);
            if (orient > 0) {return LEFT;}
            if (orient < 0) {return RIGHT;}
            return MIDDLE;
        }
    }


    LineSide Line::side(const Vec2D &p1, const Vec2D &p2, const Vec2D &p) {
        return Line(p1, p2).side(p);
    }

    double Line::find_y(double x) const {
        if (vertical) {throw GeometryException("Infinite number of solutions for y = ? in a vertical line.");}
        return a * x + b;
    }

    double Line::find_x(double y) const {
        if (vertical) {return vert_x;}
        if (a == 0) {throw GeometryException("Infinite number of solutions for x = ? in an horizontal line.");}
        return (y - b) / a;
    }

    bool Line::is_vertical() const {
        return vertical;
    }

    Vec2D Line::get_vec() const {
        if (vertical) {
            if (reversed) {return {0, -1};}
            else {return {0, 1};}
        }
        else {
            return {1, a};
        }
    }
} // Msfl2D