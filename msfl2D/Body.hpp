//
// Created by myselfleo on 26/06/2023.
//

#ifndef MSFL2D_BODY_HPP
#define MSFL2D_BODY_HPP

#include "Shape.hpp"

#include <vector>
#include <memory>

namespace Msfl2D {

    typedef unsigned long BodyId;

    /**
     * Store together shape and position data relative to its body center.
     */
    struct ShapeData {
        std::shared_ptr<Shape> shape;
        Vec2D position;

        ShapeData(std::shared_ptr<Shape> shape, Vec2D pos);
    };


    /**
     * A body is a simulation element with specific parameters (position, speed, mass, etc.).
     */
    class Body {
    public:
        /**
         * Position, or "center" of the body. It must be the average position of each shape position.
         * This value will update each time you add/remove a shape from the body.
         * Most of the time, your body will only have one shape, so its position is "obvious" (the center of the shape).
         * It may become less obvious when constructing bodies with multiple shapes.
         */
        Vec2D position;

        /**
         * Create a body with no shape. Its position will be set to (0, 0), but it's useless as it will update
         * when adding a shape.
         */
        Body() = default;


        /**
         * Add a shape to the body. Return a reference to the body so you can chain those method calls.
         */
        Body& add_shape(std::unique_ptr<Shape> shape);

        /**
         * Remove the shape at the given index. Throws GeometryException is the index does not exists.
         */
        void remove_shape(int idx);

    private:
        // Shapes are stored together with their relative position to the body center.
        // Like for vertices in ComplexPolygons, the center of a body is the average position of its shapes.
        // The constructors of the Body takes care of updating the shape positions so that they average to (0, 0).
        std::vector<ShapeData> shapes;

        /**
         * Update the center of the body so it is at the average position of each shape.
         */
        void update_center();
    };

} // Mslf2D

#endif //MSFL2D_BODY_HPP
