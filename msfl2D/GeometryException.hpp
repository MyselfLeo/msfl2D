//
// Created by myselfleo on 22/06/2023.
//

#ifndef MSFL2D_GEOMETRYEXCEPTION_HPP
#define MSFL2D_GEOMETRYEXCEPTION_HPP

#include <exception>

namespace Msfl2D {

    /**
     * This exception is raised an impossible geometrical action is effected.
     */
    class GeometryException: public std::exception {
    private:
        char* message;
    public:
        explicit GeometryException(char * msg);
        [[nodiscard]] char* what() const noexcept override;
    };

} // Msfl2D

#endif //MSFL2D_GEOMETRYEXCEPTION_HPP
