//
// Created by leo on 14/06/23.
//

#include "ConvexPolygon.hpp"

namespace Msfl2D {

    ConvexPolygon::ConvexPolygon(const std::vector<Vec2D> &vertices, Vec2D center) {
        // Check that the average of the given vertices is {0, 0}. If it is not the case, the
        // polygon is invalid and therefore cannot be constructed.
        if (ConvexPolygon::vec2D_average(vertices) != Vec2D(0, 0)) {
            throw GeometryException("The average position of the vertices is not equal to {0, 0}. It does not represent a real polygon!");
        }

        // todo: check that the polygon is convex

        this->vertices = vertices;
        this->position = center;
    }


    ConvexPolygon::ConvexPolygon(const std::vector<Vec2D> &vertices) {
        // todo: check that the polygon is convex

        // Compute the center of the polygon.
        this->position = vec2D_average(vertices);

        // Convert the absolution position of the vertices into relative ones.
        for (auto& v: vertices) {
            this->vertices.push_back(v - this->position);
        }
    }




    Segment ConvexPolygon::project(const Line &line) const {
        return Segment();
    }


    Vec2D ConvexPolygon::vec2D_average(const std::vector<Vec2D> &vectors) {
        Vec2D res = {0, 0};
        for (auto& v: vectors) {
            res += v;
        }
        return res / vectors.size();
    }
} // Msfl2D