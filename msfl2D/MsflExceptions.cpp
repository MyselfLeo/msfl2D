//
// Created by myselfleo on 22/06/2023.
//

#include <iostream>
#include "MsflExceptions.hpp"

namespace Msfl2D {
    GeometryException::GeometryException(const char *msg): message(msg) {}
    const char * GeometryException::what() const noexcept {return message;}


    SimulationException::SimulationException(const char *msg): message(msg) {}
    const char *SimulationException::what() const noexcept {return message;}


    InternalException::InternalException(const char *msg): message(msg) {}
    const char *InternalException::what() const noexcept {return message;}
} // Msfl2D