//
// Created by myselfleo on 01/07/2023.
//

#ifndef MSFL2D_COLLISIONDETECTOR_HPP
#define MSFL2D_COLLISIONDETECTOR_HPP

#include <memory>
#include "ConvexPolygon.hpp"

namespace Msfl2D {


    /**
     * Results of an SAT (Separated Axis Theorem) collision test.
     * @param collide Whether or not the tested shapes collide or not. If `false`, the next values must be ignored.
     * @param minimum_penetration_vector The vector (from shape1 to shape2) where the penetration is minimal
     * @param depth the penetration distance between the 2 shapes, along the minimum_penetration_vector
     * @param collision_point the global position where the collision force should be applied. It's either the only
     * penetrating point of the penetrating shape, or the average of the multiple penetrating points.
     */
    struct SATResult {
        bool collide;
        Vec2D minimum_penetration_vector;
        double depth;
        Vec2D collision_point;

        SATResult(bool collide, Vec2D pen_vec, double depth);

        /** Return a "no collision" SATResult */
        static SATResult no_collision();

    };

    /** This static class contains numerous methods used to detect collision between shapes. */
    class CollisionDetector {
    public:
        /**
         *
         * @param shape1
         * @param shape2
         * @return
         */
        static SATResult sat(const std::shared_ptr<ConvexPolygon>& shape1, const std::shared_ptr<ConvexPolygon>& shape2);
    };

} // Msfl2D

#endif //MSFL2D_COLLISIONDETECTOR_HPP
