//
// Created by myselfleo on 12/06/23.
//

#include "Shape.hpp"

namespace Msfl2D {
    const Vec2D &Shape::get_position() const {
        return position;
    }

    double Shape::get_rotation() const {return rotation;}
} // Msfl2D