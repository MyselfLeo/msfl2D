//
// Created by myselfleo on 14/06/23.
//

#include <cmath>
#include "ConvexPolygon.hpp"
#include "Line.hpp"
#include "GeometryException.hpp"

namespace Msfl2D {

    ConvexPolygon::ConvexPolygon(const std::vector<Vec2D> &vertices, Vec2D zero) {
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




    Segment ConvexPolygon::project(const Line &line) const {
        double min = (vertices[0] + position).project(line); // add the center of the polygon to get the global position of each vertices
        double max = min;

        // We project each vertex of the polygon. We keep the minimal and maximal point (the one closest to the line's
        // zero and the farest one)
        for (auto& v: vertices) {
            double proj = (v + position).project(line);
            if (proj < min) {min = proj;}
            if (proj > max) {max = proj;}
        }

        return {min, max};
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
        Vec2D p1 = vertices[0] + position;
        Vec2D p2 = vertices[1] + position;

        LineSide expected_side = Line::side(p1, p2, p);

        // Now, we check that the point is at the same side for every other lines.
        for (int i=1; i<vertices.size(); i++) {
            // Compute points of the line
            p1 = vertices[i] + position;
            p2 = vertices[(i+1) % vertices.size()] + position;
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
} // Msfl2D