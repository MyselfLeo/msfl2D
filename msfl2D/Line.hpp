//
// Created by myselfleo on 14/06/23.
//

#ifndef MSFL2D_LINE_HPP
#define MSFL2D_LINE_HPP

#include "Vec2D.hpp"


enum LineSide {
    MIDDLE,
    LEFT,
    RIGHT
};


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

        /**
         * Return which side is the point along the line.
         * @param p The point to test
         * @return MIDDLE if the point is on the line, LEFT or RIGHT otherwise.
         */
        LineSide side(const Vec2D& p) const;

        /**
         * which side is the point p along the line (a,b).
         * @return MIDDLE if the point is on the line, LEFT or RIGHT otherwise.
         */
        static LineSide side(const Vec2D& a, const Vec2D& b, const Vec2D& p);
    };

} // Msfl2D

#endif //MSFL2D_LINE_HPP
