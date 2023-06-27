//
// Created by myselfleo on 21/06/23.
//

#include "World.hpp"
#include "MsflExceptions.hpp"

#include <random>

namespace Msfl2D {
    World::World() {
        // init RNG generator
        std::random_device rd;
        rng_gen.seed(rd());
    }


    BodyID World::add_body(const std::shared_ptr<Body>& body) {
        BodyID id = new_id();
        bodies.insert(std::make_pair(id, body));
        return id;
    }

    void World::remove_body(BodyID id) {
        if (bodies.find(id) == bodies.end()) {throw SimulationException("Tried to remove inexistant body");}
        bodies.erase(id);
    }

    std::shared_ptr<Body> World::get_body(BodyID id) const {
        try {return bodies.at(id);}
        catch (std::out_of_range& _) {throw SimulationException("Tried to access inexistant body");}
    }

    BodyID World::new_id() {
        // Uniform distribution over every unsigned long integers
        std::uniform_int_distribution<unsigned long> distribution(1, ULONG_MAX);

        // Find an unused id. It should be
        BodyID id;
        do {
            id = distribution(rng_gen);
        } while (bodies.find(id) != bodies.end());

        return id;
    }


    int World::nb_bodies() const {
        return bodies.size();
    }

    const std::unordered_map<BodyID, std::shared_ptr<Body>> &World::get_bodies() const {
        return bodies;
    }
} // Msfl2D