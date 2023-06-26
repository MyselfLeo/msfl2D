//
// Created by myselfleo on 26/06/2023.
//

#include "Body.hpp"
#include "GeometryException.hpp"

#include <memory>

namespace Msfl2D {

    ShapeData::ShapeData(std::shared_ptr<Shape> shape, Vec2D pos): shape(std::move(shape)), position(pos) {}



    Body& Body::add_shape(std::unique_ptr<Shape> shape) {
        Vec2D rel_pos = shape->position - position;
        ShapeData data = {std::move(shape), rel_pos};
        shapes.push_back(data);

        update_center();

        return *this;
    }

    void Body::update_center() {
        // Compute the global position of the shapes, average them, and recompute their relative position to the new center

        // get sum of shapes global position
        Vec2D new_center = {0,0};
        for (auto& s: shapes) {
            new_center += s.position + position;
        }

        // average them
        new_center /= shapes.size();

        // Update the position of the shapes
        for (auto& s: shapes) {
            s.position += position - new_center;
        }

        position = new_center;
    }

    void Body::remove_shape(int idx) {
        if (idx > shapes.size() - 1) {throw GeometryException("Tried to remove an inexistant shape");}
        shapes.erase(shapes.begin() + idx);
    }


} // Mslf2D
