//
// Created by myselfleo on 27/06/2023.
//

#include "Color4.hpp"

namespace Msfl2Demo {
    Color4::Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a): r(r), g(g), b(b), a(a) {}
    Color4::Color4(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b), a(255) {}

    Color4::operator ImColor() const {
        return {r, g, b, a};
    }

    Color4::operator SDL_Color() const {
        return {r, g, b, a};
    }
} // Msfl2Demo