//
// Created by myselfleo on 26/06/2023.
//

#include "Body.hpp"
#include "MsflExceptions.hpp"

#include <memory>
#include <cmath>

namespace Msfl2D {
    Body& Body::add_shape(const std::shared_ptr<Shape>& shape) {
        shapes.push_back(shape);
        update_center();
        return *this;
    }

    Vec2D Body::get_center() const {
        return position;
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

    const std::vector<std::shared_ptr<Shape>> &Body::get_shapes() const {
        return shapes;
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

    void Body::rotate_shape(int idx, double angle) {
        if (idx > shapes.size() - 1) {throw GeometryException("Tried to update an inexistant shape");}

        // prevent overflow of rotation value
        if (angle > (M_PI * 2)) {angle -= M_PI * 2;}
        if (angle < (M_PI * -2)) {angle += M_PI * 2;}

        shapes[idx]->rotation = angle;
    }


    void Body::rotate(double angle) {
        rotate(angle, position);
    }


    void Body::rotate(double angle, const Vec2D &center) {
        for (auto& s: shapes) {
            s->rotation += angle;                            // rotate the vertices of the shapes around the shape centers

            // prevent overflow of rotation value
            if (s->rotation > (M_PI * 2)) {s->rotation -= M_PI * 2;}
            if (s->rotation < (M_PI * -2)) {s->rotation += M_PI * 2;}


            s->position = s->position.rotate(angle, center); // rotate the center of the shapes around the specified point
        }
        update_center();
    }


    void Body::reset_forces() {forces.clear();}
    void Body::register_force(Vec2D force) {forces.emplace_back(force, Vec2D(0, 0));}
    void Body::register_force(Vec2D force, Vec2D application_point) {forces.emplace_back(force, application_point);}


    void Body::apply_forces(double delta_t) {

    }


} // Mslf2D
