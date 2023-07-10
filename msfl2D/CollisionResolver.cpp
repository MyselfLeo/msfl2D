//
// Created by myselfleo on 10/07/2023.
//

#include "CollisionResolver.hpp"

namespace Msfl2D {
    void
    CollisionResolver::resolve(const SATResult &col_result) {
        if (col_result.nb_collision_points == 0) {
            std::cout << "No collision point" << std::endl;
            return;
        }

        std::shared_ptr<Body> ref_body = col_result.reference_shape->get_body();
        std::shared_ptr<Body> inc_body = col_result.incident_shape->get_body();

        // Return early as no body can move
        if (ref_body->is_static && inc_body->is_static) {return;}

        Vec2D min_pen_vec_normalised = col_result.minimum_penetration_vector.normalized();

        // first, we move the shapes so they are not intersecting.
        // todo: take mass into account
        Vec2D correction_vector = min_pen_vec_normalised * col_result.depth;
        if (ref_body->is_static) {
            inc_body->move(inc_body->get_center() - correction_vector);
        }
        else if (inc_body->is_static) {
            ref_body->move(ref_body->get_center() + correction_vector);
        }
        else {
            inc_body->move(inc_body->get_center() + correction_vector/2);
            ref_body->move(ref_body->get_center() - correction_vector/2);
        }



        // Average the bounciness of the 2 bodies so we have a value for it.
        double bounciness = (ref_body->get_bounciness() + inc_body->get_bounciness()) / 2;

        // compute the collision velocities of the 2 bodies
        Vec2D relative_velocity = inc_body->velocity - ref_body->velocity;
        Vec2D inc_body_collision_velocity = min_pen_vec_normalised * Vec2D::dot(min_pen_vec_normalised, relative_velocity);

        relative_velocity = ref_body->velocity - inc_body->velocity;
        Vec2D ref_body_collision_velocity = min_pen_vec_normalised * Vec2D::dot(min_pen_vec_normalised, relative_velocity);

        // nullify velocity of the 2 bodies
        inc_body->velocity -= inc_body_collision_velocity;
        ref_body->velocity -= ref_body_collision_velocity;


        // apply bounciness
        inc_body_collision_velocity *= bounciness;
        ref_body_collision_velocity *= bounciness;
        inc_body_collision_velocity /= col_result.nb_collision_points;
        ref_body_collision_velocity /= col_result.nb_collision_points;

        // todo: take mass into account

        if (ref_body->is_static) {
            std::cout << "only applying to ref body" << std::endl;
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(-inc_body_collision_velocity, col_result.collision_points[i]);
            }
        }
        else if (inc_body->is_static) {
            std::cout << "only applying to inc body" << std::endl;
            for (int i=0; i<col_result.nb_collision_points; i++) {
                ref_body->register_force(ref_body_collision_velocity, col_result.collision_points[i]);
            }
        }
        else {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(-inc_body_collision_velocity/2, col_result.collision_points[i]);
            }
            for (int i=0; i<col_result.nb_collision_points; i++) {
                ref_body->register_force(ref_body_collision_velocity/2, col_result.collision_points[i]);
            }
        }
    }
} // Msfl2D