//
// Created by leo on 12/06/23.
//
#include <iostream>

#include "Vec2D.hpp"
#include "Segment.hpp"

using namespace Msfl2D;


int main() {
    Segment s1 = {9, 11};
    Segment s2 = {13, 15};

    std::cout << Segment::intersection(s1, s2) << std::endl;

    return EXIT_SUCCESS;
}