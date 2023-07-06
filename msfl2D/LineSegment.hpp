//
// Created by myselfleo on 02/07/2023.
//

#ifndef MSFL2D_LINESEGMENT_HPP
#define MSFL2D_LINESEGMENT_HPP

#include "Segment.hpp"
#include "Line.hpp"

namespace Msfl2D {

    /**
     * A segment of a line. This class is line-aware.
     */
    class LineSegment {
    public:
        Segment segment;
        Line line;

        LineSegment() = default;

        LineSegment(Segment segment, Line line);

        /**
         * Construct a LineSegment from a global-space segment. This will create a line with p1 as its origin.
         */
        LineSegment(const Vec2D& p1, const Vec2D& p2);

        /**
         * Check if two segments are intersecting (i.e. they share a portion of the line).
         * Throw a GeometryException if the two segments are not on the same line.
         * @param s1 One of the 2 segments to check
         * @param S2 One of the 2 segments to check
         * @return this portion or an empty LineSegment (min = 0, max = 0) if not colliding.
         */
        static LineSegment intersection(const LineSegment& s1, const LineSegment& s2);

        /**
         * Return the intersection point between the segment and a line.
         * Throw a GeometryException if there is no intersection.
         */
        Vec2D intersection(const Line& line) const;

        /**
         * Return the position on the line of the intersection point between the segment and a line.
         * Throw a GeometryException if there is no intersection.
         */
        double intersection_grad(const Line& line) const;

        /**
         * Return the coordinates of the end points of the segment.
         */
        std::tuple<Vec2D, Vec2D> coordinates() const;

        /**
         * Return a vector with the same direction and length of the segment.
         */
        Vec2D get_vec() const;

        /**
         * Return the length of the segment
         */
        double length() const;
    };

} // Msfl2D

#endif //MSFL2D_LINESEGMENT_HPP
