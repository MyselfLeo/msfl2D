//
// Created by myselfleo on 26/06/2023.
//

#ifndef MSFL2D_BODY_HPP
#define MSFL2D_BODY_HPP

#include "Shape.hpp"

#include <vector>
#include <memory>

namespace Msfl2D {


    /**
     * A body is a simulation element with specific parameters (position, speed, mass, etc.).
     */
    class Body: public std::enable_shared_from_this<Body> {
    public:
        /**
         * Velocity vector.
         */
        Vec2D velocity = {0, 0};

        /**
         * Bodies can be made static, which mean they won't be affected by any forces. To move them, use move() instead
         * of applying forces to it.
         */
        bool is_static = false;

        /**
         * Create a body with no shape. Its position will be set to (0, 0), but it's useless as it will update
         * when adding a shape.
         */
        Body() = default;


        /**
         * Remove the center of the body, in world-space coordinates. This is its {0, 0} relative position.
         */
        Vec2D get_center() const;


        /**
         * Add a shape to the body. Return a reference to the body so you can chain those method calls.
         */
        Body& add_shape(const std::shared_ptr<Shape>& shape);

        /**
         * Remove the shape at the given index. Throws GeometryException is the index does not exists.
         */
        void remove_shape(int idx);

        /**
         * Return a pointer to one of the body's shape.
         * @param idx the index of the shape to get. Throws GeometryException is the index does not exists.
         * @return A smart pointer to the Shape.
         */
        std::shared_ptr<Shape> get_shape(int idx);

        /**
         * Return a const reference to the vector storing the shapes of this body.
         */
        const std::vector<std::shared_ptr<Shape>>& get_shapes() const;

        /**
         * Update the position of one of the body's shape. Will update the body's "position" to match the
         * center of each shapes.
         * For 1-shape bodies, this is equivalent to move(pos).
         * @param idx the index of the shape to update. Throws GeometryException is the index does not exists.
         */
        void move_shape(int idx, Vec2D pos);

        /**
         * Update the rotation of one of the body's shape.
         * For 1-shape bodies, this is equivalent to rotate(angle).
         * @param idx the index of the shape to update. Throws GeometryException is the index does not exists.
         */
        void rotate_shape(int idx, double angle);

        /**
         * Move the position of the body. Will move each shapes the same way.
         * For 1-shape bodies, this is equivalent to move_shape(0, pos).
         */
        void move(Vec2D pos);

        /**
         * Rotate the body around its center.
         * @param angle in radians
         */
        void rotate(double angle);

        /**
         * Rotate the body around a specified point (in global coordinates).
         * Its position will also be modified.
         */
        void rotate(double angle, const Vec2D& center);


        /**
         * Reset the forces applied to this body. Should be called at the beginning of every simulation steps.
         */
        void reset_forces();

        /**
         * Register a force to be applied to the body.
         * The force will virtually be applied to every point of the body. In reality, the force is multiplied by
         * the body mass, then applied to its center.
         */
        void register_force(Vec2D force);

        /**
         * Register a force to be applied to the body. The given position must be relative to the body center.
         * You can obtain this position simply by doing absolute_force_position - body.position
         */
        void register_force(Vec2D force, Vec2D application_point);

        /**
         * Apply the forces previously registered (using register_force) to the body.
         * @param delta_t time to simulate, in seconds.
         */
        void apply_forces(double delta_t);


        /**
         * Return the bounciness of the body.
         */
        double get_bounciness() const;

        /**
         * Set the bounciness of the body.
         * Throw SimulationException if the value is not between 0 & 1.
         */
        void set_bounciness(double b);

        /**
         * Set m of the body.
         * Doesn't do anything if the body is static.
         */
        void set_mass(double m);


        double get_mass() const;




    private:
        // Position, or "center" of the body. It must be the average position of each shape position.
        // This value will update each time you add/remove a shape from the body.
        // Most of the time, your body will only have one shape, so its position is "obvious" (the center of the shape).
        // It may become less obvious when constructing bodies with multiple shapes.
        Vec2D position = {0, 0};

        // Like for vertices in ComplexPolygons, the center of a body is the average position of its shapes.
        // The constructors of the Body takes care of updating body center.
        std::vector<std::shared_ptr<Shape>> shapes;


        // Collision resolution (among other things, like gravity application) will add a force to this vector along
        // with the application point of the force (relative to the body center).
        std::vector<std::tuple<Vec2D, Vec2D>> forces;

        /**
         * A value between 0 & 1 representing how bouncy the body is. 0 = not bouncy at all, 1 = as bouncy as possible.
         */
        double bounciness = 0.7;

        // Might prefer to use the inverse of the mass to speed up the computing
        double mass = 1;


        /**
         * Update the center of the body so it is at the average position of each shape.
         */
        void update_center();
    };

} // Mslf2D

#endif //MSFL2D_BODY_HPP
