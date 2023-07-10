//
// Created by myselfleo on 14/06/23.
//

#include <cmath>
#include "ConvexPolygon.hpp"
#include "Line.hpp"
#include "MsflExceptions.hpp"

namespace Msfl2D {

    ConvexPolygon::ConvexPolygon(const std::vector<Vec2D> &vertices, Vec2D zero) {
        this->rotation = 0;
        this->body = nullptr;

        // Add the given vertices to the shape, adding the zero coordinates to it (make it absolute).
        // At that point, the vertices coordinates are absolute; we'll make them relative to the center later
        // (we first need to compute it)
        for (auto& v: vertices) {
            this->vertices.push_back(v + zero);
        }

        // Compute the center of the polygon.
        this->position = vec2D_average(this->vertices);

        // Update the vertices so that they are relative to the center of the ConvexPolygon
        for (auto& v: this->vertices) {
            v -= this->position;
        }

        if (!is_convex()) {
            throw GeometryException("The vertices do not form a convex polygon.");
        }
    }


    ConvexPolygon::ConvexPolygon(const std::vector<Vec2D> &vertices) {
        this->rotation = 0;
        this->body = nullptr;

        // Compute the center of the polygon.
        this->position = vec2D_average(vertices);

        // Convert the absolution position of the vertices into relative ones.
        for (auto& v: vertices) {
            this->vertices.push_back(v - this->position);
        }

        if (!is_convex()) {
            throw GeometryException("The vertices do not form a convex polygon.");
        }
    }



    ConvexPolygon::ConvexPolygon(unsigned int vertex_nb, double circumradius, Vec2D center) {
        if (vertex_nb < 3) {throw GeometryException("Cannot create a ConvexPolygon with less than 3 vertices.");}
        if (circumradius <= 0) {throw GeometryException("The circumradius of a ConvexPolygon must be > 0");}

        this->position = center;
        this->rotation = 0;
        this->body = nullptr;

        for (int i=0; i<vertex_nb; i++) {
            double rad = i * -2 * M_PI / vertex_nb;
            Vec2D dir_vec = {cos(rad), sin(rad)};
            this->vertices.push_back(dir_vec * circumradius);
        }
    }




    LineSegment ConvexPolygon::project(const Line &line) const {
        double min = (get_global_vertex(0)).project(line); // add the center of the polygon to get the global position of each vertices
        double max = min;

        // We project each vertex of the polygon. We keep the minimal and maximal point (the one closest to the line's
        // zero and the farest one)
        for (int i=0; i<nb_vertices(); i++) {
            double proj = get_global_vertex(i).project(line);
            if (proj < min) {min = proj;}
            if (proj > max) {max = proj;}
        }

        return {{min, max}, line};
    }


    Vec2D ConvexPolygon::vec2D_average(const std::vector<Vec2D> &vectors) {
        Vec2D res = {0, 0};
        for (auto& v: vectors) {
            res += v;
        }
        return res / vectors.size();
    }


    bool ConvexPolygon::is_point_inside(const Vec2D &p) const {
        // We check that the point is at the same side for each lines of the polygon.
        // Compute line for vertex 0 to vertex 1, so we have the side that should be the same for every other lines.
        Vec2D p1 = get_global_vertex(0);
        Vec2D p2 = get_global_vertex(1);

        LineSide expected_side = Line::side(p1, p2, p);

        // Now, we check that the point is at the same side for every other lines.
        for (int i=1; i<vertices.size(); i++) {
            // Compute points of the line
            p1 = get_global_vertex(i);
            p2 = get_global_vertex((i+1) % vertices.size());
            LineSide side = Line::side(p1, p2, p);
            // Only exit if the point is outside of the polygon.
            // The Shape class expects that "is_point_inside" returns true if the point is on the shape.
            if (side != expected_side && side != LineSide::MIDDLE) {return false;}
        }

        return true;
    }

    bool ConvexPolygon::is_convex() const {
        // The polygon is convex if every inner angle acute.
        // We need the counterclockwise angle (as we are iterating over the vertices clockwise).
        // For that, we use the dot product, the determinant of the 2 vectors along with atan2; see below.

        // we just iterate over each vertex i, and we use as vectors the segments [i i-1] and [i i+1].
        for (unsigned int i=0; i<vertices.size(); i++) {
            // modulo operator is cringe so i do it cringier. For some reason it works for i+1 tho
            unsigned int prev_i;
            if (i == 0) {prev_i = vertices.size() - 1;}
            else {prev_i = i - 1;}

            Vec2D a = vertices[prev_i] - vertices[i];
            Vec2D b = vertices[(i+1) % vertices.size()] - vertices[i];

            // in radians, > 0.
            double dot = Vec2D::dot(a, b);
            double det = Vec2D::det(a, b);

            double angle = atan2(det, dot);

            // angle is > 0 is the inner angle is acute, < 0 other wise.
            if (angle < 0) {return false;}
        }

        return true;
    }

    Vec2D &ConvexPolygon::get_vertex(int idx) {
        if (idx > vertices.size() - 1) {throw GeometryException("Tried to access an inexistant vertex");}
        return vertices[idx];
    }

    Vec2D ConvexPolygon::get_global_vertex(int idx) const {
        if (idx > vertices.size() - 1) {throw GeometryException("Tried to access an inexistant vertex");}
        Vec2D res = vertices[idx].rotate(rotation); // apply rotation
        return res + position;                            // apply position

    }

    const Vec2D &ConvexPolygon::get_const_vertex(int idx) const {
        if (idx > vertices.size() - 1) {throw GeometryException("Tried to access an inexistant vertex");}
        return vertices[idx];
    }

    int ConvexPolygon::nb_vertices() const {
        return vertices.size();
    }
} // Msfl2D