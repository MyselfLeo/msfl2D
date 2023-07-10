//
// Created by myselfleo on 12/06/23.
//

#include "Shape.hpp"
#include "Body.hpp"

namespace Msfl2D {
    const Vec2D &Shape::get_position() const {
        return position;
    }

    double Shape::get_rotation() const {return rotation;}

    std::shared_ptr<Body> Shape::get_body() const {
        return body;
    }
} // Msfl2D