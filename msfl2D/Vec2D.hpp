//
// Created by myselfleo on 5/19/23.
//

#ifndef P2D_VEC2D_HPP
#define P2D_VEC2D_HPP

#include <iostream>


namespace Msfl2D {

    // Predefinition of Line to prevent circular import
    class Line;

    /**
     * A Vector in a 2D space.
     */
    class Vec2D {
    public:
        double x;
        double y;

        /** Vector of length zero */
        const static Vec2D ZERO;

        /** Default constructor */
        Vec2D(): x(0), y(0) {}

        /** Base constructor */
        Vec2D(double xx, double yy): x(xx), y(yy) {}

        /**
         * Returns the norm of the vector, i.e. its length.
         */
        double norm() const;

        /**
         * Returns a normalized version of the vector, i.e. same direction but with a length of 1.
         */
        Vec2D normalized() const;

        /**
         * Returns the dot product of 2 vectors.
         * @param v1 a Vec2D
         * @param v2 a Vec2D
         * @return the dot product of the two vectors
         */
        static double dot(const Vec2D& v1, const Vec2D& v2);

        /**
         * Returns the cross product of 2 vectors.
         * The cross product of 2D vectors is not a vector like in 3D but a scalar.
         * This function is equivalent to Vec2D::det().
         * @param v1 a Vec2D
         * @param v2 a Vec2D
         * @return the cross product of the two vectors
         */
         static double cross(const Vec2D& v1, const Vec2D& v2);


        /**
         * Returns the square of the distance between 2 points represented by the 2 Vec2D.
         * @param v1 first point
         * @param v2 second point
         * @return the square of the distance
         */
        static double distance_squared(const Vec2D& v1, const Vec2D& v2);

        /**
         * Returns the distance between 2 points represented by the 2 Vec2D.
         * @param v1 first point
         * @param v2 second point
         * @return the distance
         */
        static double distance(const Vec2D& v1, const Vec2D& v2);

        /**
         * Return the determinant of the 2x2 matrix formed by the 2 vectors stacked next to each other as column vectors.
         * This function is equivalent to Vec2D::cross().
         * @return The determinant of the 2x2 matrix (v1 v2)
         */
        static double det(const Vec2D& v1, const Vec2D& v2);


        /**
         * Project the point onto the line, return the distance from the origin of the line.
         */
        double project(const Line& line) const;


        /**
         * Return the rotated vector by the given angle counterclock-wise.
         * @param angle the angle of rotation, in radians.
         * @return The rotated vector.
         */
        Vec2D rotate(double angle) const;

        /**
         * Return the rotated vector around the given point by the given angle counterclock-wise.
         * @param angle the angle of rotation, in radians.
         * @param center the center of the rotation
         * @return The rotated vector.
         */
        Vec2D rotate(double angle, const Vec2D& center) const;


        // Operator overloading for base types
        bool operator==(const Vec2D& other) const;
        bool operator!=(const Vec2D& other) const;

        Vec2D operator+(const Vec2D& other) const;
        Vec2D& operator+=(const Vec2D& other);

        Vec2D operator-(const Vec2D& other) const;
        Vec2D& operator-=(const Vec2D& other);

        Vec2D operator*(double s) const;
        Vec2D& operator*=(double s);

        Vec2D operator/(double s) const;
        Vec2D& operator/=(double s);

        Vec2D operator-() const;


        // std::cout operators
        friend std::ostream& operator<<(std::ostream& os, const Vec2D& v);

    private:


    };
} // Msfl2D


#endif //P2D_VEC2D_HPP
