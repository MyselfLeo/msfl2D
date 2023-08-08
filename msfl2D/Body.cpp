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
        shape->body = shared_from_this();
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
    void Body::register_force(Vec2D force) {
        if (is_static) {return;}
        forces.emplace_back(force * get_mass(), Vec2D(0, 0));
    }
    void Body::register_force(Vec2D force, Vec2D application_point) {
        if (is_static) {return;}
        forces.emplace_back(force, application_point);
    }


    void Body::apply_forces(double delta_t) {
        // apply each forces, modifying velocity & inertia
        for (auto& f: forces) {
            Vec2D force = std::get<0>(f);
            Vec2D point_of_application = std::get<1>(f);

            velocity += force * delta_t / get_mass();


            if (point_of_application != get_center()) {
                // compute angular velocity
                double torque = Vec2D::cross(force, -point_of_application);

                // moment of inertia. Lets assume its a circle. // todo: maybe really compute this ?
                // lets compute the moment of inertia of a circle that has the same mass and density as the body
                //todo: replace mass by area when we'll compute it (to compute mass using density)
                double fallacious_circle_radius = sqrt(get_mass() / M_PI);
                double moment_inertia = M_PI * pow(fallacious_circle_radius, 4) / 4;
                double angular_acceleration = torque / moment_inertia;

                angular_vel += angular_acceleration * delta_t;
            }
        }

        // apply velocity & inertia
        move(position + (velocity * delta_t));

        // apply angular velocity
        rotate(angular_vel * delta_t);
    }

    double Body::get_bounciness() const {
        return bounciness;
    }

    void Body::set_bounciness(double b) {
        if (b < 0 || b > 1) {throw SimulationException("The bounciness must be a number between 0 & 1.");}
        bounciness = b;
    }

    void Body::set_mass(double m) {
        if (is_static) {this->mass = 0;}
        else {this->mass = m;}
    }

    double Body::get_mass() const {
        if (is_static) {return 0;}
        return mass;
    }

    void Body::set_friction(double f) {
        if (f < 0 || f > 1) {throw SimulationException("The friction must be a number between 0 & 1.");}
        friction = f;
    }

    double Body::get_friction() const {
        return friction;
    }

    Vec2D Body::get_point_velocity(const Vec2D &point) const {
        double radius = point.norm();
        double tangential_speed = radius * angular_vel;
        Vec2D tangent = Vec2D(-point.y, point.x).normalized();

        return tangent * tangential_speed + velocity;
    }

    int Body::get_nb_collisions() const {return nb_colliding_points;}


} // Mslf2D
