//
// Created by myselfleo on 10/07/2023.
//

#include "CollisionResolver.hpp"
#include "MsflExceptions.hpp"

namespace Msfl2D {
    void CollisionResolver::resolve(const SATResult &col_result, double delta_t) {
        // Return early if CollisionDetector lies (its not our problem)
        if (col_result.nb_collision_points == 0) {return;}

        // Return early as no body can move
        if (col_result.ref_body->is_static && col_result.inc_body->is_static) {return;}

        collision(col_result, delta_t);

        friction(col_result, delta_t);

        // Move the shapes so they are not intersecting.
        separate(col_result);
    }





    void CollisionResolver::point_collision(const SATResult &col_result, double delta_t, int point_idx) {
        if (point_idx >= col_result.nb_collision_points) {
            throw InternalException("point_collision: Given a point index higher than the collision point count");
        }

        std::shared_ptr<Body> ref_body = col_result.ref_body;
        std::shared_ptr<Body> inc_body = col_result.inc_body;

        Vec2D point = col_result.collision_points[point_idx];

        // Mass of the reference body is distributed over each point of the current collision
        double reference_mass = ref_body->get_mass() / col_result.nb_collision_points;
        // Mass of the incident body is distributed over each point on which it lies during this update step.
        double incident_mass = inc_body->get_mass() / inc_body->get_nb_collisions();
        double mass_sum = reference_mass + incident_mass;

        Vec2D min_pen_vec = col_result.minimum_penetration_vector;

        // We use the body's velocity, not the points one.
        // The angular velocity of the collision point will be used later in the collision resolution process.
        Vec2D ref_velocity = ref_body->velocity;
        Vec2D inc_velocity = inc_body->velocity;

        // We keep only the component of each velocity that goes in the direction of the collision
        // Compute the component of each body velocity in the direction of the collision
        Vec2D ref_coll_velocity = min_pen_vec * Vec2D::dot(min_pen_vec, ref_velocity);
        Vec2D inc_coll_velocity = min_pen_vec * Vec2D::dot(min_pen_vec, inc_velocity);



        /*// Return early if the two points are receding in the axis of penetration
        double current_dist = Vec2D::distance_squared(ref_body->get_center(), inc_body->get_center());
        double next_dist = Vec2D::distance_squared(
                ref_body->get_center() + ref_coll_velocity * APPROACHING_PRECISION,
                inc_body->get_center() + inc_coll_velocity * APPROACHING_PRECISION);
        if (current_dist < next_dist) {return;}*/



        // Compute linear momentum of the collision
        Vec2D ref_lin_momentum = ref_coll_velocity * reference_mass;
        Vec2D inc_lin_momentum = inc_coll_velocity * incident_mass;
        Vec2D total_lin_momentum = ref_lin_momentum + inc_lin_momentum;





        // Find the final velocity of the points, after the collision, in order
        // to compute the forces to be applied
        Vec2D velocity_diff = ref_coll_velocity - inc_coll_velocity;



        Vec2D ref_final_velocity;
        Vec2D inc_final_velocity;

        Vec2D ref_final_lin_momentum;
        Vec2D inc_final_lin_momentum;

        if (col_result.ref_body->is_static) {
            ref_final_velocity = {0, 0};
            inc_final_velocity = -total_lin_momentum / mass_sum;
        }
        else if (col_result.inc_body->is_static) {
            inc_final_velocity = {0, 0};
            ref_final_velocity = -total_lin_momentum / mass_sum;
        }
        else {
            ref_final_velocity = (total_lin_momentum - velocity_diff * incident_mass) / mass_sum;
            inc_final_velocity = (total_lin_momentum + velocity_diff * reference_mass) / mass_sum;
        }

        Vec2D ref_velocity_change = ref_final_velocity - ref_coll_velocity;
        Vec2D inc_velocity_change = inc_final_velocity - inc_coll_velocity;


        double bounciness = (ref_body->get_bounciness() + inc_body->get_bounciness()) / 2;




        // Tangential direction of the colliding point (will be 0 is the incident body is not rotating)
        Vec2D inc_ang_momentum = inc_body->get_point_angular_momentum(point - inc_body->get_center());
        Vec2D collision_point_tangent = inc_ang_momentum.normalized();

        Vec2D



        // Linear component of the force experienced by the ref body due to the inc body (and vice-versa)
        // We'll build a new force vector with those values as linear components, with added angular component
        Vec2D ref_lin_force = ref_velocity_change * reference_mass * bounciness / delta_t;
        Vec2D inc_lin_force = inc_velocity_change * incident_mass * bounciness / delta_t;


        // Apply collision force
        ref_body->register_force(ref_force, point - ref_body->get_center());
        inc_body->register_force(inc_force, point - inc_body->get_center());


        /*if (col_result.ref_body->is_static) {
            col_result.inc_body->register_force(inc_force, point - col_result.ref_body->get_center());
        }
        else if (col_result.inc_body->is_static) {
            Vec2D force = ref_force / col_result.nb_collision_points;
            for (int i=0; i<col_result.nb_collision_points; i++) {
                Vec2D rel_pos = col_result.collision_points[i] - ref_body->get_center();
                ref_body->register_force(force, rel_pos);
            }
        }
        else {
            for (int i=0; i<col_result.nb_collision_points; i++) {
                Vec2D rel_pos_ref = col_result.collision_points[i] - ref_body->get_center();
                Vec2D rel_pos_inc = col_result.collision_points[i] - inc_body->get_center();
                inc_body->register_force(inc_force / col_result.nb_collision_points, rel_pos_inc);
                ref_body->register_force(ref_force / col_result.nb_collision_points, rel_pos_ref);
            }
        }*/
    }





    void CollisionResolver::collision(const SATResult &col_result, double delta_t) {
        for (int i=0; i<col_result.nb_collision_points; i++) {
            point_collision(col_result, delta_t, i);
        }
    }







    void CollisionResolver::friction(const SATResult &col_result, double delta_t) {
        std::shared_ptr<Body> ref_body = col_result.reference_shape->get_body();
        std::shared_ptr<Body> inc_body = col_result.incident_shape->get_body();


    }


    void CollisionResolver::separate(const SATResult &col_result) {
        std::shared_ptr<Body> ref_body = col_result.reference_shape->get_body();
        std::shared_ptr<Body> inc_body = col_result.incident_shape->get_body();

        Vec2D min_pen_vec_normalised = col_result.minimum_penetration_vector.normalized();

        // Move the shapes so they are not intersecting.
        Vec2D correction_vector = min_pen_vec_normalised * col_result.depth;

        // The correction is weighted by the velocity of each body towards the other.
        Vec2D ref_towards_inc = (inc_body->get_center() - ref_body->get_center()).normalized();
        Vec2D inc_towards_ref = (ref_body->get_center() - inc_body->get_center()).normalized();

        double ref_rel_vel = Vec2D::dot(ref_towards_inc, ref_body->velocity);
        if (ref_rel_vel < 0) {ref_rel_vel = 0;}
        double inc_rel_vel = Vec2D::dot(inc_towards_ref, inc_body->velocity);
        if (inc_rel_vel < 0) {inc_rel_vel = 0;}

        // Both should not be negative, because in that case the collision wouldn't be detected.
        double speed_sum = ref_rel_vel + inc_rel_vel;

        if (speed_sum == 0) {
            std::cout << "speed sum equal to 0" << std::endl;
            return;
        }

        if (ref_body->is_static) {
            inc_body->move(inc_body->get_center() - correction_vector);
        }
        else if (inc_body->is_static) {
            ref_body->move(ref_body->get_center() + correction_vector);
        }
        else {
            double ref_body_speed_ratio = ref_rel_vel / speed_sum;
            double inc_body_speed_ratio = 1 - ref_body_speed_ratio;
            std::cout << "ref_body_speed_ratio: " << ref_body_speed_ratio << std::endl;
            inc_body->move(inc_body->get_center() - correction_vector * inc_body_speed_ratio);
            ref_body->move(ref_body->get_center() + correction_vector * ref_body_speed_ratio);
        }
    }
} // Msfl2D