//
// Created by myselfleo on 22/06/2023.
//

#ifndef MSFL2D_MSFLEXCEPTIONS_HPP
#define MSFL2D_MSFLEXCEPTIONS_HPP

#include <exception>

namespace Msfl2D {

    /**
     * This exception is raised when an impossible geometrical action is performed.
     */
    class GeometryException: public std::exception {
    private:
        char* message;
    public:
        explicit GeometryException(char * msg);
        [[nodiscard]] char* what() const noexcept override;
    };


    /**
     * This exception is raised an impossible simulation action is effected.
     */
    class SimulationException: public std::exception {
    private:
        char* message;
    public:
        explicit SimulationException(char * msg);
        [[nodiscard]] char* what() const noexcept override;
    };

} // Msfl2D

#endif //MSFL2D_MSFLEXCEPTIONS_HPP
