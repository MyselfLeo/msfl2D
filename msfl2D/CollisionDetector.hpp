//
// Created by myselfleo on 01/07/2023.
//

#ifndef MSFL2D_COLLISIONDETECTOR_HPP
#define MSFL2D_COLLISIONDETECTOR_HPP

#include <memory>
#include "ConvexPolygon.hpp"

namespace Msfl2D {


    const int MAX_COLLISION_POINTS = 10;

    /**
     * Results of an SAT (Separated Axis Theorem) collision test.
     * @param collide Whether or not the tested shapes collide or not. If `false`, the following values must be ignored.
     * @param minimum_penetration_vector The vector (from shape1 to shape2) for which the penetration is minimal
     * @param depth the penetration distance between the 2 shapes, along the minimum_penetration_vector.
     *              this is the distance to move one of the shape from the other (along the minimum_penetration_vector)
     *              so that they barely touch.
     * @param nb_collision_points the number of collision points. There may be 1 or 2.
     * @param collision_point The collision points. They are the points that collided first, hence the limit of 2.
     */
    struct SATResult {
        bool collide;
        Vec2D minimum_penetration_vector;
        double depth;
        int nb_collision_points;
        Vec2D collision_point[MAX_COLLISION_POINTS];
        std::vector<LineSegment> collision_sides;
        LineSegment reference_side;

        SATResult(bool collide, Vec2D pen_vec, double depth, int nb_col_points, Vec2D col_points[MAX_COLLISION_POINTS], std::vector<LineSegment> col_sides, LineSegment ref_side);

        /** Return a "no collision" SATResult */
        static SATResult no_collision();

    };

    /** This static class contains numerous methods used to detect collision between shapes. */
    class CollisionDetector {
    public:
        /**
         * Perform a SAT test to compute collision information about two shapes.
         */
        static SATResult sat(const std::shared_ptr<ConvexPolygon>& shape1, const std::shared_ptr<ConvexPolygon>& shape2);
    };

} // Msfl2D

#endif //MSFL2D_COLLISIONDETECTOR_HPP
