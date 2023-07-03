//
// Created by myselfleo on 02/07/2023.
//

#include "LineSegment.hpp"
#include "MsflExceptions.hpp"

namespace Msfl2D {
    LineSegment::LineSegment(Segment segment, Line line): segment(segment), line(line) {}


    LineSegment::LineSegment(const Vec2D &p1, const Vec2D &p2) {
        line = {p1, p2};
        segment = {0, (p2-p1).norm()};
    }


    std::tuple<Vec2D, Vec2D> LineSegment::coordinates() const {
        Vec2D origin = line.get_origin();
        Vec2D dir_vec = line.get_vec();
        return {
            origin + dir_vec * segment.min,
            origin + dir_vec * segment.max
        };
    }

    double LineSegment::length() const {return segment.length();}

    LineSegment LineSegment::intersection(const LineSegment &s1, const LineSegment &s2) {
        if (!Line::overlap(s1.line, s2.line)) {throw GeometryException("LineSegments must be on the same line to compute intersection.");}

        return {Segment::intersection(s1.segment, s2.segment), s1.line};
    }

    Vec2D LineSegment::intersection(const Line &l) const {
        Vec2D line_intersection;
        try {line_intersection = Line::intersection(line, l);}
        catch (GeometryException& e) {throw GeometryException("No intersection between the line & the segment.");}

        // check that the intersection point is inside the segment (and not outside)
        double dist = l.get_grad_coo(line_intersection);
        if (dist >= segment.min && dist <= segment.max) {
            return line_intersection;
        }
        else {
            throw GeometryException("No intersection between the line & the segment.");
        }
    }

    Vec2D LineSegment::get_vec() const {
        return line.get_vec() * segment.length();
    }
} // Msfl2D