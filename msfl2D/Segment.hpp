//
// Created by leo on 12/06/23.
//

#ifndef MSFL2D_SEGMENT_HPP
#define MSFL2D_SEGMENT_HPP

namespace Msfl2D {

    /**
     * @brief A segment of a line.
     */
    class Segment {
    public:
        double min;
        double max;

        /**
         * @brief Default constructor, creates an "empty" segment (min 0, max 0)
         */
        Segment();

        /**
         * @brief Base constructor of the Segment.
         * @param p1 One of the two end points of the segment
         * @param p2 One of the two end points of the segment
         */
        Segment(double p1, double p2);

        /**
         * Check if two segments are intersecting (i.e. they share a portion of the line).
         * @param s1 One of the 2 segments to check
         * @param S2 One of the 2 segments to check
         * @return this portion or an empty Segment (min = 0, max = 0) if not colliding.
         */
        static Segment intersection(const Segment& s1, const Segment& s2);


        // Allow printing the segment data to the command-line
        friend std::ostream& operator<<(std::ostream& os, const Segment& seg);
    };


} // Msfl2D


#endif //MSFL2D_SEGMENT_HPP
