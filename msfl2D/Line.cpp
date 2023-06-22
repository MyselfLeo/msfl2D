//
// Created by myselfleo on 14/06/23.
//

#include "Line.hpp"

namespace Msfl2D {
    Line::Line(double slope, const Vec2D &zero) : slope(slope), zero(zero) {}

    LineSide Line::side(const Vec2D &p) const {
        double orient = Vec2D::cross(Vec2D(1, slope), p - zero);
        if (orient > 0) {return LEFT;}
        if (orient < 0) {return RIGHT;}
        return MIDDLE;
    }


    LineSide Line::side(const Vec2D &a, const Vec2D &b, const Vec2D &p) {
        double orient = Vec2D::cross(b-a, p-a);
        if (orient > 0) {return LEFT;}
        if (orient < 0) {return RIGHT;}
        return MIDDLE;
    }
} // Msfl2D