//
// Created by myselfleo on 01/07/2023.
//

#include "CollisionDetector.hpp"

#include <utility>
#include "MsflExceptions.hpp"

namespace Msfl2D {

    SATResult SATResult::no_collision() {
        return {false, Msfl2D::Vec2D(), 0, 0, nullptr, {}}; // pen_vec and depth values are not important
    }

    SATResult::SATResult(bool collide, Vec2D pen_vec, double depth, int nb_col_points, Vec2D col_points[2], LineSegment ref_side):
        collide(collide),
        minimum_penetration_vector(pen_vec),
        depth(depth),
        nb_collision_points(nb_col_points),
        reference_side(ref_side) {
        if (col_points != nullptr) {
            for (int i=0; i<2; i++) {
                collision_points[i] = col_points[i];
            }
        }
    }


    SATResult CollisionDetector::sat(std::shared_ptr<ConvexPolygon> shape1, std::shared_ptr<ConvexPolygon> shape2) {
        // 1. We find the reference side. This is the side of a ConvexPolygon for which the penetration value is the least.
        //    This is the vector of minimal penetration.
        //    We also conserve the penetration depth for that penetration vector; it will be used later to find the collision points.
        //    Finally, this step also allow to find if there is no collision: if we find a axis for which the penetration is 0, then
        //    this axis is a separating axis and the shapes do not collide. (we return early from that)

        Vec2D minimum_penetration_vector {0, 0};         // initialisation value, will be changed
        double depth = -1;                                      // initialisation value, will be changed
        LineSegment reference_side;
        double min_dist_from_ref_side;
        std::shared_ptr<ConvexPolygon> reference_polygon;       // Polygon owning the reference side
        std::shared_ptr<ConvexPolygon> incident_polygon;        // Polygon "entering" the reference polygon



        // Lord pray for my soul as I use a goto statement
        // Seems useful in that case though, we test shape1 against shape2, then we switch shape1 & shape2 and do the same test again
        bool has_switched = false;

        other_shape_test:
        // The axis of minimal penetration will the normal of one of the shapes sides. So, we iterate of the sides of each shape.
        int i;
        for (i=0; i<shape1->nb_vertices(); i++) {
            // Get the side we're checking
            LineSegment tested_side = LineSegment(
                    shape1->get_global_vertex(i),
                    shape1->get_global_vertex((i+1) % shape1->nb_vertices())
                    );

            Vec2D side_vec = tested_side.get_vec();
            Vec2D proj_axis = {side_vec.y, -side_vec.x}; // normal of the side

            // Project the shapes onto a line with this orientation (passing through the origin because i said so)
            Line proj_line = Line::from_director_vector({0, 0}, proj_axis);
            LineSegment proj_shape_1 = shape1->project(proj_line);
            LineSegment proj_shape_2 = shape2->project(proj_line);

            LineSegment penetration = LineSegment::intersection(proj_shape_1, proj_shape_2);

            // No intersection => we found a separating axis
            if (penetration.length() == 0) {
                return SATResult::no_collision();
            }



            // We need to compute the minimal distance between the vertices of the potential incident polygon
            // & the potential reference side. This is to prevent a parallel side to the desired side to be considered
            // reference side.
            double min_dist = -1;
            for (int j=0; j<shape2->nb_vertices(); j++) {
                double dist = shape2->get_global_vertex(j).distance_squared(tested_side.line);
                if (dist < min_dist || min_dist == -1) {min_dist = dist;}
            }


            // Potential reference side
            if (depth == -1 || penetration.length() < depth) {
                depth = penetration.length();
                minimum_penetration_vector = proj_axis;
                reference_side = tested_side;
                reference_polygon = shape1;
                incident_polygon = shape2;
                min_dist_from_ref_side = min_dist;
            }

            else if (penetration.length() == depth) {
                if (min_dist < min_dist_from_ref_side) {
                    depth = penetration.length();
                    minimum_penetration_vector = proj_axis;
                    reference_side = tested_side;
                    reference_polygon = shape1;
                    incident_polygon = shape2;
                    min_dist_from_ref_side = min_dist;
                }
            }


        }

        if (!has_switched) {
            has_switched = true;
            auto tmp = shape1;
            shape1 = shape2;
            shape2 = tmp;
            goto other_shape_test;
        }




        // 2. Now that we have the reference side, we clip the incident_polygon to the reference_side side planes.
        //    I.e we find the intersections between each side of the incident polygon and the 2 lines normal to the
        //    reference side crossing with the reference side extremities. This will give us a list of points, to which
        //    we also include the vertices of the incident side.
        //    This list of points will contain the collision points. We'll filter them in the next step.


        std::vector<Vec2D> points;
        std::vector<LineSegment> sides;
        for (i=0; i<incident_polygon->nb_vertices(); i++) {
            points.push_back(incident_polygon->get_global_vertex(i));
        }


        // The 2 normal lines coming from the end points of the reference side.
        std::tuple<Vec2D, Vec2D> end_points = reference_side.coordinates();
        Line l1 = Line::from_director_vector(std::get<0>(end_points), minimum_penetration_vector);
        Line l2 = Line::from_director_vector(std::get<1>(end_points), minimum_penetration_vector);

        // Find the intersections
        for (i=0; i<incident_polygon->nb_vertices(); i++) {
            // Get the side we're checking
            LineSegment tested_side = LineSegment(
                    incident_polygon->get_global_vertex(i),
                    incident_polygon->get_global_vertex((i+1) % incident_polygon->nb_vertices())
                    );

            // Check for intersection with one of the normal lines
            try {
                points.push_back(tested_side.intersection(l1));
                sides.push_back(tested_side);
            }
            catch (GeometryException& e) {}
            try {
                points.push_back(tested_side.intersection(l2));
                sides.push_back(tested_side);
            }
            catch (GeometryException& e) {}
        }


        // 3. Now, we have a list of potential collision points. We'll only keep:
        //    - The ones that "crossed" the reference side (i.e the ones on the RIGHT or directly on the side)
        //    - The ones the farest from that side (they are the ones that crossed it first)

        int nb_points = 0;
        double max_distance = 0;
        Vec2D col_points[2];


        for (auto& p: points) {
            // Only consider points that crossed the reference side.
            // ConvexPolygons are represented clockwise, meaning that a point to the right of one of its
            // side "crossed it", if coming from the exterior.
            if (reference_side.line.side(p) == LineSide::LEFT) {continue;}

            double current_distance = p.distance_squared(reference_side.line);

            // only keep the farest points from the reference side
            if (nb_points == 0 || current_distance > max_distance) {
                col_points[0] = p;
                max_distance = current_distance;
                nb_points = 1;
            }
            else if (current_distance == max_distance && nb_points < 2) {
                col_points[1] = p;
                nb_points = 2;
            }
        }



        // 4. Now, we have everything for the SATResult

        return {
                true,
                minimum_penetration_vector,
                depth,
                nb_points,
                col_points,
                reference_side
                };
    }
} // Msfl2D