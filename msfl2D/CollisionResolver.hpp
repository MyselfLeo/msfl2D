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
        static constexpr double APPROACHING_PRECISION = 0.001;

        /**
         * Resolve the collision
         */
        static void resolve(const SATResult& col_result);

    };

} // Msfl2D

#endif //MSFL2D_COLLISIONRESOLVER_HPP
