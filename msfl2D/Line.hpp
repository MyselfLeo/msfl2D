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
     * An infinite line in a 2D space.
     */
    class Line {
    public:
        /**
         * Construct a line using its a and b parameters, such as the line equation is y = ax + b.
         */
        Line(double a, double b);

        /*
         * Construct a line using 2 different points on that line.
         * Throw a GeometryException if the given points are equal.
         */
        Line(const Vec2D& p1, const Vec2D& p2);

        /**
         * Return which side is the point along the line.
         * @param p The point to test
         * @return MIDDLE if the point is on the line, LEFT or RIGHT otherwise.
         */
        LineSide side(const Vec2D& p) const;

        /**
         * which side is the point p along the line (p1,p2).
         * @return MIDDLE if the point is on the line, LEFT or RIGHT otherwise.
         */
        static LineSide side(const Vec2D& p1, const Vec2D& p2, const Vec2D& p);

        /**
         * Return the y coordinate of the point of the line at the x coordinate
         */
        double find_y(double x) const;

        /**
         * Return the x coordinate of the point of the line at the y coordinate
         */
        double find_x(double y) const;

        /**
         * Return true if the line is perpendicular to the x axis.
         */
        bool is_vertical() const;

        /**
         * Return a normalised vector with the same orientation as the line.
         */
        Vec2D get_vec() const;

        /**
         * Return the slope of the line.
         */
        double get_slope() const;

    private:
        // non-vertical lines
        double a;
        double b;

        // vertical lines
        bool vertical;
        double vert_x;
        bool reversed; // false = neg to pos, true = pos to neg

    };

} // Msfl2D

#endif //MSFL2D_LINE_HPP
