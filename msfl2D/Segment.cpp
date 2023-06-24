//
// Created by myselfleo on 12/06/23.
//

#include "Segment.hpp"
#include "Line.hpp"

using namespace Msfl2D;

Segment::Segment() : min(0), max(0) {}

Segment::Segment(double p1, double p2) {
    // We sort the end points so the user don't have to. (the user is me)
    if (p1 > p2) {
        min = p2;
        max = p1;
    }
    else {
        min = p1;
        max = p2;
    }
}

Segment Segment::intersection(const Segment &s1, const Segment &s2) {
    // Find the "first" and "second" segment: the "first" is the one with the lowest "min".
    Segment first;
    Segment second;
    if (s1.min < s2.min) {
        first = s1;
        second = s2;
    }
    else {
        first = s2;
        second = s1;
    }

    // Now we check if the min point of the second segment is before the max point of the first one.
    // If so, the segments are intersecting between those two points.
    if (first.max > second.min) {
        return {first.max, second.min};
    }
    else {
        return {0, 0};
    }
}


std::tuple<Vec2D, Vec2D> Segment::coordinates(const Line& line) const {
    Vec2D origin = line.get_origin();
    Vec2D dir_vec = line.get_vec();
    return {
        origin + dir_vec * min,
        origin + dir_vec * max
    };
}



std::ostream &operator<<(std::ostream &os, const Segment &seg) {
    os << "[" << seg.min << ", " << seg.max << "]";
    return os;
}