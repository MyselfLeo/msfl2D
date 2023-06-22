//
// Created by myselfleo on 22/06/2023.
//

#include <iostream>
#include "GeometryException.hpp"

namespace Msfl2D {
    GeometryException::GeometryException(char *msg): message(msg) {}
    char * GeometryException::what() const noexcept {return message;}

} // Msfl2D