//
// Created by leo on 5/19/23.
//

#ifndef P2D_VEC2D_HPP
#define P2D_VEC2D_HPP

#include <iostream>


namespace Msfl2D {


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
         * @param v1 a Vec2D
         * @param v2 a Vec2D
         * @return the cross product of the two vectors
         */
         static double cross(const Vec2D& v1, const Vec2D& v2);

         /**
          * Returns the cross product of a vector and a scalar.
          * Basically, for a vector A and a scalar s, it computes (s * A.y, - s * A.x).
          * @param v a Vec2D
          * @param s a scalar
          * @return the cross product of a vector and a scalar
          */
          static Vec2D cross(const Vec2D& v, double s);

          /**
          * Returns the cross product of a vector and a scalar.
          * Basically, for a vector A and a scalar s, it computes (-s * A.y, s * A.x).
          * @param s a scalar
          * @param v a Vec2D
          * @return the cross product of a vector and a scalar
          */
          static Vec2D cross(double s, const Vec2D& v);


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
