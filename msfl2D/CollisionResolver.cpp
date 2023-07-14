//
// Created by myselfleo on 10/07/2023.
//

#include "CollisionResolver.hpp"

namespace Msfl2D {
    void CollisionResolver::resolve(const SATResult &col_result, double delta_t) {
        if (col_result.nb_collision_points == 0) {
            return;
        }

        std::shared_ptr<Body> ref_body = col_result.reference_shape->get_body();
        std::shared_ptr<Body> inc_body = col_result.incident_shape->get_body();

        // Return early as no body can move
        if (ref_body->is_static && inc_body->is_static) {return;}

        // Return early if the two shapes are receding
        double current_dist = Vec2D::distance_squared(ref_body->get_center(), inc_body->get_center());
        double next_dist = Vec2D::distance_squared(
                ref_body->get_center() + ref_body->velocity * APPROACHING_PRECISION,
                inc_body->get_center() + inc_body->velocity * APPROACHING_PRECISION);
        if (current_dist < next_dist) {return;}

        Vec2D min_pen_vec_normalised = col_result.minimum_penetration_vector.normalized();




        // Move the shapes so they are not intersecting.
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








        // Compute collision force in the case of a total restitution (the final values will be weighted by the bounciness)
        // 1. Compute the final velocities of the 2 bodies using their initial velocities, using the conservation of
        //    momentum principle.

        /*Vec2D ref_body_momentum = ref_body->velocity * ref_body->get_mass();
        Vec2D inc_body_momentum = inc_body->velocity * inc_body->get_mass();
        Vec2D total_momentum = ref_body_momentum + inc_body_momentum;
        Vec2D velocity_diff = ref_body->velocity - inc_body->velocity;
        double total_sum = ref_body->get_mass() + inc_body->get_mass();
        Vec2D ref_body_final_velocity = (total_momentum - velocity_diff * inc_body->get_mass()) / (total_sum);
        Vec2D inc_body_final_velocity = (total_momentum - velocity_diff * ref_body->get_mass()) / (total_sum);

        // 2. We can now compute the collision force using the difference in velocity of each body.
        Vec2D ref_velocity_change = ref_body->velocity - ref_body_final_velocity;
        Vec2D inc_velocity_change = inc_body->velocity - inc_body_final_velocity;

        double bounciness = (ref_body->get_bounciness() + inc_body->get_bounciness()) / 2;

        // Force experienced by the ref body due to the inc body
        Vec2D ref_force = -inc_velocity_change * inc_body->get_mass() * bounciness / delta_t;
        // The inverse
        Vec2D inc_force = ref_velocity_change * ref_body->get_mass() * bounciness / delta_t;*/




        // Compute the component of each body velocity in the direction of the collision
        Vec2D ref_coll_velocity = min_pen_vec_normalised * Vec2D::dot(min_pen_vec_normalised, ref_body->velocity);
        Vec2D inc_coll_velocity = min_pen_vec_normalised * Vec2D::dot(min_pen_vec_normalised, inc_body->velocity);

        Vec2D ref_coll_momentum = ref_coll_velocity * ref_body->get_mass();
        Vec2D inc_coll_momentum = inc_coll_velocity * inc_body->get_mass();
        Vec2D total_momentum = ref_coll_momentum + inc_coll_momentum;

        Vec2D velocity_diff = ref_coll_velocity - inc_coll_velocity;

        double mass_sum = ref_body->get_mass() + inc_body->get_mass();

        Vec2D ref_body_final_velocity;
        Vec2D inc_body_final_velocity;
        if (ref_body->is_static) {
            ref_body_final_velocity = {0, 0};
            inc_body_final_velocity = -total_momentum / mass_sum;
        }
        else if (inc_body->is_static) {
            inc_body_final_velocity = {0, 0};
            ref_body_final_velocity = -total_momentum / mass_sum;
        }
        else {
            ref_body_final_velocity = (total_momentum - velocity_diff * inc_body->get_mass()) / mass_sum;
            inc_body_final_velocity = (total_momentum + velocity_diff * ref_body->get_mass()) / mass_sum;
        }


        Vec2D ref_velocity_change = ref_body_final_velocity - ref_coll_velocity;
        Vec2D inc_velocity_change = inc_body_final_velocity - inc_coll_velocity;

        double bounciness = (ref_body->get_bounciness() + inc_body->get_bounciness()) / 2;

        // Force experienced by the ref body due to the inc body (and vice-versa)
        Vec2D ref_force;
        Vec2D inc_force;
        if (ref_body->is_static) {
            ref_force = {0, 0};
            inc_force = inc_velocity_change * inc_body->get_mass() * bounciness / delta_t;
        }
        else if (inc_body->is_static) {
            ref_force = ref_velocity_change * ref_body->get_mass() * bounciness / delta_t;
            inc_force =  {0, 0};
        }
        else {
            //ref_force = inc_velocity_change * inc_body->get_mass() * bounciness / delta_t;
            //inc_force = ref_velocity_change * ref_body->get_mass() * bounciness / delta_t;
            ref_force = ref_velocity_change * ref_body->get_mass() * bounciness / delta_t;
            inc_force = inc_velocity_change * inc_body->get_mass() * bounciness / delta_t;
        }






        // Apply collision force
        if (ref_body->is_static) {
            Vec2D force = inc_force / col_result.nb_collision_points;
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(force, col_result.collision_points[i]);
            }
        }
        else if (inc_body->is_static) {
            Vec2D force = ref_force / col_result.nb_collision_points;
            for (int i=0; i<col_result.nb_collision_points; i++) {
                ref_body->register_force(force, col_result.collision_points[i]);
            }
        }
        else {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                inc_body->register_force(inc_force / col_result.nb_collision_points, col_result.collision_points[i]);
                ref_body->register_force(ref_force / col_result.nb_collision_points, col_result.collision_points[i]);
            }
        }
    }
} // Msfl2D