//
// Created by leo on 14/06/23.
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
         * Construct a ConvexPolygon with a list of vertices relative to the polygon's center.
         * Note that in that case, the average position of the vertices should be {0, 0}. This constructor checks for that
         * condition and throw a GeometryException if not met.
         * @param vertices list of vertices relative to the ConvexPolygon's center, in clockwise order.
         * @param center center of the polygon (= average absolute position of the vertices) of the polygon
         */
        ConvexPolygon(const std::vector<Vec2D>& vertices, Vec2D center);

        /**
         * Construct a ConvexPolygon with a list of vertices in a 2D space.
         * This constructor computes the polygon's center and convert the absolute coordinates of the polygon into
         * relative (to the polygon center) ones.
         * @param vertices list of vertices in the absolute space, in clockwise order.
         */
        explicit ConvexPolygon(const std::vector<Vec2D>& vertices);



        ~ConvexPolygon() = default;

        Segment project(const Msfl2D::Line &line) const override;


    private:
        std::vector<Vec2D> vertices;

        /**
         * Returns the average position of the Vec2Ds.
         */
        static Vec2D vec2D_average(const std::vector<Vec2D>& vectors);
    };

} // Msfl2D

#endif //MSFL2D_CONVEXPOLYGON_HPP
