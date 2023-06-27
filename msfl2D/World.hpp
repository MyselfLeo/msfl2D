//
// Created by myselfleo on 21/06/23.
//

#ifndef MSFL2D_WORLD_HPP
#define MSFL2D_WORLD_HPP

#include <unordered_map>
#include <memory>
#include <random>

#include "Body.hpp"

namespace Msfl2D {

    typedef unsigned long BodyID;

    /**
     * Instance of a simulation space, containing one or more bodies and with specific parameters.
     * The bodies are identified by a unique ID.
     */
    class World {
    public:
        World();

        /**
         * Add a body to the world and return its newly created BodyID.
         * @return
         */
        BodyID add_body(const std::shared_ptr<Body>&);


        /**
         * Remove a body from the World.
         * Throws SimulationException if the body does not exists.
         * @param id the id of the body
         */
        void remove_body(BodyID id);


        /**
         * Return a smart pointer to the body with the given id.
         * Throws SimulationException if the body does not exists.
         */
        std::shared_ptr<Body> get_body(BodyID id) const;

        /**
         * Return the number of bodies of the simulation
         */
        int nb_bodies() const;

        /**
         * Return a read-only reference to the map containing the bodies of this world.
         */
        const std::unordered_map<BodyID, std::shared_ptr<Body>>& get_bodies() const;


    private:
        std::unordered_map<BodyID, std::shared_ptr<Body>> bodies;

        // random number generator
        std::mt19937 rng_gen;

        /**
         * Return a new unused BodyID.
         */
        BodyID new_id();
    };

} // Msfl2D

#endif //MSFL2D_WORLD_HPP
