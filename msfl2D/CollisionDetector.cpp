//
// Created by myselfleo on 01/07/2023.
//

#include "CollisionDetector.hpp"

namespace Msfl2D {


    SATResult::SATResult(bool collide, Vec2D pen_vec, double depth):
        collide(collide),
        minimum_penetration_vector(pen_vec),
        depth(depth) {}

    SATResult SATResult::no_collision() {
        return {false, Msfl2D::Vec2D(), 0}; // pen_vec and depth values are not important
    }


    SATResult CollisionDetector::sat(const std::shared_ptr<ConvexPolygon> &shape1, const std::shared_ptr<ConvexPolygon> &shape2) {
        // source: https://www.sevenson.com.au/programming/sat/

        // For each side of each shape, we take its perpendicular axis and we project the 2 shapes on it.
        // If we find an axis where the projections don't touch, we found a separating axis and the shapes are not
        // colliding.
        // We keep track of the axis and penetration value we found to return the minimum_penetration_vector and the
        // penetration depth.
        Vec2D minimum_penetration_vector {0, 0};
        double depth = -1; // initialisation value, will be changed

        // Iterate over the sides of shape1 first, then shape2 if needed.
        int i;
        for (i=0; i<shape1->nb_vertices(); i++) {
            // Get the projection axis
            Vec2D side = shape1->get_global_vertex((i+1) % shape1->nb_vertices()) - shape1->get_global_vertex(i);
            Vec2D proj_axis = {side.y, -side.x};

            // Project the shapes onto a line with this orientation (passing through the origin because i said so)
            Line proj_line = Line::from_director_vector({0, 0}, proj_axis);
            Segment proj_shape_1 = shape1->project(proj_line);
            Segment proj_shape_2 = shape2->project(proj_line);

            Segment penetration = Segment::intersection(proj_shape_1, proj_shape_2);

            // No intersection => we found a separating axis
            if (penetration.length() == 0) {
                return SATResult::no_collision();
            }

            // keep track of the minimum depth
            if (depth == -1 || penetration.length() < depth) {
                depth = penetration.length();
                minimum_penetration_vector = proj_axis;
            }
        }

        for (i=0; i<shape2->nb_vertices(); i++) {
            // Get the projection axis
            Vec2D side = shape2->get_global_vertex((i+1) % shape2->nb_vertices()) - shape2->get_global_vertex(i);
            Vec2D proj_axis = {side.y, -side.x};

            // Project the shapes onto a line with this orientation (passing through the origin because i said so)
            Line proj_line = Line::from_director_vector({0, 0}, proj_axis);
            Segment proj_shape_1 = shape1->project(proj_line);
            Segment proj_shape_2 = shape2->project(proj_line);

            Segment penetration = Segment::intersection(proj_shape_1, proj_shape_2);

            // No intersection => we found a separating axis
            if (penetration.length() == 0) {
                return SATResult::no_collision();
            }

            // keep track of the minimum depth
            if (depth == -1 || penetration.length() < depth) {
                depth = penetration.length();
                minimum_penetration_vector = proj_axis;
            }
        }


        return {true, minimum_penetration_vector, depth};
    }
} // Msfl2D