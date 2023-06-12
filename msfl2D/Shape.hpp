//
// Created by leo on 12/06/23.
//

#ifndef MSFL2D_SHAPE_HPP
#define MSFL2D_SHAPE_HPP

namespace Msfl2D {

    /**
     * @brief Base class for the shapes used in the physic engine.
     * Derived shapes must implement the `project` method, which is used for performing SAT
     * (Separating Axis Theorem) collision calculation.
     */
    class Shape {
    public:

    protected:


    private:
    };

} // Msfl2D

#endif //MSFL2D_SHAPE_HPP
