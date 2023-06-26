//
// Created by myselfleo on 12/06/23.
//

#ifndef MSFL2D_SHAPE_HPP
#define MSFL2D_SHAPE_HPP

#include "Segment.hpp"
#include "Line.hpp"

namespace Msfl2D {
    /**
     * Base class for the shapes used in the physic engine.
     * Derived shapes must implement the `project` method, which is used for performing SAT
     * (Separating Axis Theorem) collision calculation.
     */
    class Shape {
    public:
        Vec2D position;

        virtual ~Shape() = 0;

    protected:

        /**
         * Projects the shape onto the line
         * @param line the line to project onto
         * @return the segment formed by the projection.
         */
        virtual Segment project(const Line& line) const = 0;

        /**
         * Check if the given point is inside or outside the shape. This function should return true if the
         * point is directly on the polygon.
         * @param p the point to check
         * @return true if inside, false if outside
         */
        virtual bool is_point_inside(const Vec2D& p) const = 0;
    private:
    };

} // Msfl2D

#endif //MSFL2D_SHAPE_HPP
