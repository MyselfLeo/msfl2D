//
// Created by myselfleo on 10/07/2023.
//

#ifndef MSFL2D_COLLISIONRESOLVER_HPP
#define MSFL2D_COLLISIONRESOLVER_HPP

#include "CollisionDetector.hpp"
#include "Body.hpp"

namespace Msfl2D {

    /** This static class is used to resolve collision by separating the bodies and applying correct forces to them. */
    class CollisionResolver {
    public:

        /** Precision of approaching detection */
        static constexpr double APPROACHING_PRECISION = 0.01;

        /**
         * Resolve the collision. This is done in 3 steps:
         * - Compute collision forces
         * - Separate the intersecting bodies
         * - Apply friction
         */
        static void resolve(const SATResult& col_result, double delta_t);

    private:

        /** Separate the 2 colliding bodies */
        static void separate(const SATResult& col_result);

        /** Resolve collision force */
        static void collision(const SATResult& col_result, double delta_t);

        /** Resolve collision for the specified collision point in col_result */
        static void point_collision(const SATResult& col_result, double delta_t, int point_idx);

        /** Resolve friction to the collision */
        static void friction(const SATResult& col_result, double delta_t);
    };

} // Msfl2D

#endif //MSFL2D_COLLISIONRESOLVER_HPP
