//
// Created by myselfleo on 10/07/2023.
//

#include "CollisionResolver.hpp"

namespace Msfl2D {
    void CollisionResolver::resolve(const SATResult &col_result) {
        if (col_result.nb_collision_points == 0) {
            return;
        }

        std::shared_ptr<Body> ref_body = col_result.reference_shape->get_body();
        std::shared_ptr<Body> inc_body = col_result.incident_shape->get_body();

        // Return early as no body can move
        if (ref_body->is_static && inc_body->is_static) {return;}

        // return early if the two shapes are receding
        double current_dist = Vec2D::distance_squared(ref_body->get_center(), inc_body->get_center());
        double next_dist = Vec2D::distance_squared(
                ref_body->get_center() + ref_body->velocity * APPROACHING_PRECISION,
                inc_body->get_center() + inc_body->velocity * APPROACHING_PRECISION);
        if (current_dist < next_dist) {return;}

        Vec2D min_pen_vec_normalised = col_result.minimum_penetration_vector.normalized();

        // first, we move the shapes so they are not intersecting.
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


        // Now, we nullify the relative velocity of one-another
        Vec2D relative_velocity = inc_body->velocity - ref_body->velocity; // velocity of inc relative to ref
        Vec2D projected_rel_velocity = min_pen_vec_normalised * Vec2D::dot(min_pen_vec_normalised, relative_velocity);
        if (ref_body->is_static) {inc_body->velocity -= projected_rel_velocity;}
        else if (inc_body->is_static) {ref_body->velocity += projected_rel_velocity;}

        else if (inc_body->get_mass() > ref_body->get_mass()) {
            double ratio = ref_body->get_mass() / inc_body->get_mass();
            inc_body->velocity -= projected_rel_velocity * (1 - ratio);
            ref_body->velocity += projected_rel_velocity * ratio;
        }
        else {
            double ratio = inc_body->get_mass() / ref_body->get_mass();
            inc_body->velocity -= projected_rel_velocity * ratio;
            ref_body->velocity += projected_rel_velocity * (1 - ratio);
        }

        // Compute collision force
        double bounciness = (ref_body->get_bounciness() + inc_body->get_bounciness()) / 2;
        Vec2D collision_force = projected_rel_velocity * bounciness * (ref_body->get_mass() + inc_body->get_mass());

        /*
        std::cout << "projected_rel_velocity: " << projected_rel_velocity << std::endl;
        std::cout << "bounciness: " << bounciness << std::endl;
        std::cout << "collision force: " << collision_force << std::endl;
        std::cout << "mass sum: " << (ref_body->get_mass() + inc_body->get_mass()) << std::endl;
         */


        if (ref_body->is_static) {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(-collision_force, col_result.collision_points[i]);
            }
        }
        else if (inc_body->is_static) {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                ref_body->register_force(collision_force, col_result.collision_points[i]);
            }
        }
        else {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(-collision_force / inc_body->get_mass(), col_result.collision_points[i]);
                ref_body->register_force(collision_force / ref_body->get_mass(), col_result.collision_points[i]);
            }
        }
    }
} // Msfl2D