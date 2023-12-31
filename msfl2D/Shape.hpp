//
// Created by myselfleo on 12/06/23.
//

#ifndef MSFL2D_SHAPE_HPP
#define MSFL2D_SHAPE_HPP

#include "Line.hpp"
#include "LineSegment.hpp"

#include <memory>

namespace Msfl2D {
    // pre-declare body because the compiler wants it so bad
    class Body;

    /**
     * Base class for the shapes used in the physic engine.
     * Derived shapes must implement the `project` method, which is used for performing SAT
     * (Separating Axis Theorem) collision calculation.
     *
     * You cannot updated its position or rotation directly; use Body.move_shape() & Body.rotate_shape() instead.
     */
    class Shape {
    public:
        virtual ~Shape() = default;

        const Vec2D& get_position() const;
        double get_rotation() const;

        std::shared_ptr<Body> get_body() const;

        /**
         * Check if the given point is inside or outside the shape. This function should return true if the
         * point is directly on the polygon.
         * @param p the point to check
         * @return true if inside, false if outside
         */
        virtual bool is_point_inside(const Vec2D& p) const = 0;

        /**
         * Projects the shape onto the line
         * @param line the line to project onto
         * @return the segment formed by the projection.
         */
        virtual LineSegment project(const Line& line) const = 0;

    protected:
        friend class Body;

        // Body owning this shape, set by the body.
        std::shared_ptr<Body> body{};

        Vec2D position;
        double rotation{}; // in radians
    };

} // Msfl2D

#endif //MSFL2D_SHAPE_HPP
