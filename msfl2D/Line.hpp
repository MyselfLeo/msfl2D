//
// Created by leo on 14/06/23.
//

#ifndef MSFL2D_LINE_HPP
#define MSFL2D_LINE_HPP

#include "Vec2D.hpp"

namespace Msfl2D {

    /**
     * An infinite line in a 2D space, defined by its slope and a point on the line.
     * This point is considered the zero of the line.
     */
    class Line {
    public:
        double slope;
        Vec2D zero;

        Line(double slope, const Vec2D &zero);
    };

} // Msfl2D

#endif //MSFL2D_LINE_HPP
