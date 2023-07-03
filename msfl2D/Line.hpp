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
     * An infinite line in a 2D space. It is represented by 2 points.
     * A line also has a graduation along itself. The origin of this graduation is the first point given when
     * constructing the object. The graduation has the same "dimension" has the global space; the point at the graduation
     * 1 is 1 unit far from the origin. The direction of the graduation is the same as the line (i.e direction of the
     * director vector).
     */
    class Line {
    public:

        Line() = default;

        /**
         * Construct a line using 2 different points on that line.
         * Throw a GeometryException if the given points are equal.
         */
        Line(const Vec2D& p1, const Vec2D& p2);

        /**
         * Returns a line using a point and a director vector.
         * The second point is just the point + the vector.
         */
        static Line from_director_vector(const Vec2D& p, const Vec2D& dir_vec);

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
         * Return the slope of the line. This is the 'a' in the equation y = ax + b
         */
        double get_slope() const;

        /**
         * Return the y value for x = 0. This is the 'b' in the equation y = ax + b.
         * Don't confuse this function with get_origin(), which return the coordinates of the point on the line
         * where the graduation along the line is starting.
         */
        double get_zero() const;

        /**
         * Return the coordinates of the line's origin in the global space. This is the origin of the graduation
         * along the line. Do not confuse this function with get_zero(), which return the y value of the line for x = 0.
         */
        Vec2D get_origin() const;

        /**
         * Return the coordinates of the point on the line at the given graduation.
         * @param g the graduation, i.e the signed distance from the origin.
         * @return the point of the line at the given graduation.
         */
        Vec2D get_coo_grad(double g) const;

        /**
         * Return the intersection point between two lines.
         * Throw GeometryException if the two lines are collinear.
         */
        static Vec2D intersection(const Line& l1, const Line& l2);

        /**
         * Return whether the given point is on the line or not.
         */
        bool has(const Vec2D& p) const;

        /**
         * Return whether the 2 lines are overlapping (i.e they are the same
         * in geometrical terms, but are represented by different values).
         */
        static bool overlap(const Line& l1, const Line& l2);

        // std::cout operators
        friend std::ostream& operator<<(std::ostream& os, const Line& l);

    private:
        // A line is represented by 2 different points.
        Vec2D p1;
        Vec2D p2;
    };

} // Msfl2D

#endif //MSFL2D_LINE_HPP
