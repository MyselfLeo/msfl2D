//
// Created by leo on 12/06/23.
//
#include <iostream>

#include "msfl2D/Vec2D.hpp"

using namespace Msfl2D;




int main() {
    Vec2D vs[3] = {{0, 1}, {1, 0}, {-2, -2}};

    Vec2D av = {0, 0};

    for (auto& v: vs) {
        av += v;
    }
    av / 3;

    std::cout << av << std::endl;

    return EXIT_SUCCESS;
}