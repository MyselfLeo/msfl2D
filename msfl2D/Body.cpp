//
// Created by myselfleo on 26/06/2023.
//

#include "Body.hpp"
#include "GeometryException.hpp"

#include <memory>

namespace Msfl2D {
    Body& Body::add_shape(const std::shared_ptr<Shape>& shape) {
        shapes.push_back(shape);
        update_center();
        return *this;
    }



    void Body::update_center() {
        // get sum of shapes global position
        Vec2D new_center = {0,0};
        for (auto& s: shapes) {
            new_center += s->position;
        }

        // average them
        new_center /= shapes.size();
        position = new_center;
    }

    void Body::remove_shape(int idx) {
        if (idx > shapes.size() - 1) {throw GeometryException("Tried to remove an inexistant shape");}
        shapes.erase(shapes.begin() + idx);
    }

    std::shared_ptr<Shape> Body::get_shape(int idx) {
        if (idx > shapes.size() - 1) {throw GeometryException("Tried to access an inexistant shape");}
        return shapes[idx];
    }

    void Body::move_shape(int idx, Vec2D pos) {
        if (idx > shapes.size() - 1) {throw GeometryException("Tried to update an inexistant shape");}
        shapes[idx]->position = pos;
        update_center();
    }

    void Body::move(Vec2D pos) {
        // Compute the displacement; we'll add it to each shape position so they move the same way.
        // That way, the center of the body will be exactly where we want it.
        Vec2D displacement = pos - position;

        for (auto& s: shapes) {
            s->position += displacement;
        }

        position = pos;
    }




} // Mslf2D
