//
// Created by myselfleo on 22/06/2023.
//

#include <iostream>
#include "MsflExceptions.hpp"

namespace Msfl2D {
    GeometryException::GeometryException(const char *msg): message(msg) {}
    char * GeometryException::what() const noexcept {return const_cast<char *>(message);}


    SimulationException::SimulationException(const char *msg): message(msg) {}
    char *SimulationException::what() const noexcept {return const_cast<char *>(message);}
} // Msfl2D