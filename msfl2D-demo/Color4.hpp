//
// Created by myselfleo on 27/06/2023.
//

#ifndef MSFL2D_COLOR4_HPP
#define MSFL2D_COLOR4_HPP

#include "imgui.h"
#include "SDL2/SDL.h"

namespace Msfl2Demo {

    /**
     * Custom Color4 class that can be implicitly converted to SDL_Color or ImColor
     */
    class Color4 {
    public:
        unsigned char r, g, b, a;

        Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        Color4(unsigned char r, unsigned char g, unsigned char b);

        explicit operator ImColor() const;
        explicit operator SDL_Color() const;
    };

} // Msfl2Demo

#endif //MSFL2D_COLOR4_HPP
