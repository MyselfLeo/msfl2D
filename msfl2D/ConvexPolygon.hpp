//
// Created by myselfleo on 14/06/23.
//

#ifndef MSFL2D_CONVEXPOLYGON_HPP
#define MSFL2D_CONVEXPOLYGON_HPP

#include <vector>
#include "Shape.hpp"

namespace Msfl2D {

    /**
     * A convex polygon represented as a list of points relative to the polygon's center.
     * A convex polygon only has inner angles equal or inferior to 180 degrees. Trying to construct a concave polygon
     * will result in a GeometryException being raised.
     * The convex aspect of the polygon is important in order to use the Separated Axis Theorem for collision detection.
     */
    class ConvexPolygon: public Shape {
    public:
        /**
         * Construct a ConvexPolygon with a list of vertices relative to a given point (the zero point).
         * Most of the time, the center of the resulting ConvexPolygon will NOT be the given zero. This is because
         * the center is computed as the average position of the vertices; but you may give vertices whose average is
         * not (0, 0).
         *
         * @param vertices list of vertices relative to the given zero, in clockwise order.
         * @param zero point to which the vertices are relative to.
         */
        ConvexPolygon(const std::vector<Vec2D>& vertices, Vec2D zero);

        /**
         * Construct a ConvexPolygon with a list of vertices in a 2D space.
         * This constructor computes the polygon's center and convert the absolute coordinates of the polygon into
         * relative (to the polygon center) ones.
         * @param vertices list of vertices in the absolute space, in clockwise order.
         */
        explicit ConvexPolygon(const std::vector<Vec2D>& vertices);

        /**
         * Construct a **regular** ConvexPolygon with the given number of vertex, the distance between
         * the center and the vertices (the circumradius) and the center position;
         */
         ConvexPolygon(unsigned int vertex_nb, double circumradius, Vec2D center);

        ~ConvexPolygon() override = default;

        LineSegment project(const Line &line) const override;

        bool is_point_inside(const Vec2D& p) const override;

        /**
         * Return a reference to the polygon's vertex at the given index.
         * If the index is too great, this method throws a GeometryException.
         * @param idx index of the vertex to get
         * @return a reference to the vertex
         */
        Vec2D& get_vertex(int idx);

        /**
         * Return the global position of the given vertex, i.e with the shape's rotation and position taken into account.
         * If the index is too great, this method throws a GeometryException.
         */
        Vec2D get_global_vertex(int idx) const;

        /**
         * Return a const reference to the polygon's vertex at the given index.
         * If the index is too great, this method throws a GeometryException.
         * @param idx index of the vertex to get
         * @return a reference to the vertex
         */
        const Vec2D& get_const_vertex(int idx) const;

        /**
         * Return the number of vertices of this polygon.
         */
        int nb_vertices() const;


    private:
        std::vector<Vec2D> vertices;

        /**
         * Returns the average position of the Vec2Ds.
         */
        static Vec2D vec2D_average(const std::vector<Vec2D>& vectors);

        /**
         * Check if self is convex.
         * @return true if self is convex, false otherwise
         */
         bool is_convex() const;
    };

} // Msfl2D

#endif //MSFL2D_CONVEXPOLYGON_HPP
