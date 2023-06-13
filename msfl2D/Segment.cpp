//
// Created by leo on 12/06/23.
//

#include "Segment.hpp"

Msfl2D::Segment::Segment() : min(0), max(0) {}

Msfl2D::Segment::Segment(double p1, double p2) {
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

Msfl2D::Segment Msfl2D::Segment::intersection(const Msfl2D::Segment &s1, const Msfl2D::Segment &s2) {
    // Find the "first" and "second" segment: the "first" is the one with the lowest "min".
    Msfl2D::Segment first;
    Msfl2D::Segment second;
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

std::ostream &Msfl2D::operator<<(std::ostream &os, const Msfl2D::Segment &seg) {
    os << "[" << seg.min << ", " << seg.max << "]";
    return os;
}
