//
// Created by myselfleo on 01/07/2023.
//

#include "CollisionDetector.hpp"

#include <utility>
#include "MsflExceptions.hpp"

namespace Msfl2D {

    SATResult SATResult::no_collision() {
        return {false, Msfl2D::Vec2D(), 0, 0, nullptr, {}, {}}; // pen_vec and depth values are not important
    }

    SATResult::SATResult(bool collide, Vec2D pen_vec, double depth, int nb_col_points, Vec2D col_points[MAX_COLLISION_POINTS], std::vector<LineSegment> col_sides, LineSegment ref_side):
        collide(collide),
        minimum_penetration_vector(pen_vec),
        depth(depth),
        nb_collision_points(nb_col_points),
        reference_side(ref_side),
        collision_sides(std::move(col_sides)) {
        if (col_points != nullptr) {
            for (int i=0; i<MAX_COLLISION_POINTS; i++) {
                collision_points[i] = col_points[i];
            }
        }
    }


    SATResult CollisionDetector::sat(const std::shared_ptr<ConvexPolygon> &shape1, const std::shared_ptr<ConvexPolygon> &shape2) {

        // 1. We find the reference side. This is the side of a ConvexPolygon for which the penetration value is the least.
        //    This is the vector of minimal penetration.
        //    We also conserve the penetration depth for that penetration vector; it will be used later to find the collision points.
        //    Finally, this step also allow to find if there is no collision: if we find a axis for which the penetration is 0, then
        //    this axis is a separating axis and the shapes do not collide. (we return early from that)

        Vec2D minimum_penetration_vector {0, 0};         // initialisation value, will be changed
        double depth = -1;                                      // initialisation value, will be changed
        LineSegment reference_side;
        std::shared_ptr<ConvexPolygon> reference_polygon;       // Polygon owning the reference side
        std::shared_ptr<ConvexPolygon> incident_polygon;        // Polygon "entering" the reference polygon



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

            // keep track of the minimum depth
            if (depth == -1 || penetration.length() < depth) {
                depth = penetration.length();
                minimum_penetration_vector = proj_axis;
                reference_side = tested_side;
                reference_polygon = shape1;
                incident_polygon = shape2;
            }
        }
        for (i=0; i<shape2->nb_vertices(); i++) {
            // Get the side we're checking
            LineSegment tested_side = LineSegment(
                    shape2->get_global_vertex(i),
                    shape2->get_global_vertex((i+1) % shape2->nb_vertices())
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

            // keep track of the minimum depth
            if (depth == -1 || penetration.length() < depth) {
                depth = penetration.length();
                minimum_penetration_vector = proj_axis;
                reference_side = tested_side;
                reference_polygon = shape2;
                incident_polygon = shape1;
            }
        }



        // 2. Now that we have the reference side, we clip the incident_polygon to the reference_side side planes.
        //    I.e we find the intersections between each side of the incident polygon and the 2 lines normal to the
        //    reference side crossing with the reference side extremities. This will give us a list of points, to which
        //    we also include the vertices of the incident side.
        //    This list of points will contain the collision points. We'll filter them in the next step.


        std::vector<Vec2D> points;
        std::vector<LineSegment> sides;
        /*for (i=0; i<incident_polygon->nb_vertices(); i++) { // todo: uncomment
            points.push_back(incident_polygon->get_global_vertex(i));
        }*/

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
        //    - The ones that are exactly depth far from the reference side
        //    - The ones that are INSIDE the reference shape.

        int nb_points = 0;
        Vec2D col_points[MAX_COLLISION_POINTS];


        for (auto& p: points) {
            if (nb_points == MAX_COLLISION_POINTS) {break;}
            /*
            if (reference_polygon->is_point_inside(p)) {
                col_points[nb_points] = p;
                nb_points += 1;
            }*/
            col_points[nb_points] = p;
            nb_points += 1;
        }



        // 4. Now, we have everything for the SATResult

        return {
                true,
                minimum_penetration_vector,
                depth,
                nb_points,
                col_points,
                sides,
                reference_side
                };
    }
} // Msfl2D