//
// Created by myselfleo on 21/06/23.
//

#include "World.hpp"
#include "MsflExceptions.hpp"
#include "CollisionDetector.hpp"
#include "CollisionResolver.hpp"

#include <random>
#include <climits>



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



    void World::update(double delta_t) {
        // Steps:
        // 1. Update body (apply forces)
        // 2. Reset forces and recompute them
        // 3. Resolve collision

        // This order might seem weird, but resolving collision AFTER updating the body prevent a situation
        // where the shapes are overlapping after calling update().
        // They overlap after step 1, then are separated in step 3.


        // Update each body with the forces computed in the last update and the friction of the environment
        for (auto& b: bodies) {
            b.second->apply_forces(delta_t);
            b.second->velocity *= (1 - friction * delta_t);
        }

        nb_collision_points = 0;

        // Reset forces & apply the constant
        // force (most of the time, gravity) to every body
        for (auto& b: bodies) {
            b.second->reset_forces();       // Clear forces from the last step
            b.second->register_force(constant_force);
        }



        // todo: collision detection & resolution
        // todo: should be based on shapes & not bodies

        // temporary: compute each pairs of bodies
        std::vector<std::tuple<std::shared_ptr<Body>, std::shared_ptr<Body>>> pairs;
        for (auto& b1: bodies) {
            for (auto& b2: bodies) {
                if (b1.first < b2.first) {
                    pairs.emplace_back(b1.second, b2.second);
                }
            }
        }

        // check for collision, resolve if needed
        for (auto& p: pairs) {
            std::shared_ptr<ConvexPolygon> bs1 = (const std::shared_ptr<Msfl2D::ConvexPolygon> &) std::get<0>(
                    p)->get_shapes()[0];
            std::shared_ptr<ConvexPolygon> bs2 = (const std::shared_ptr<Msfl2D::ConvexPolygon> &) std::get<1>(
                    p)->get_shapes()[0];
            SATResult collision_data = CollisionDetector::sat(bs1, bs2);

            if (collision_data.collide) {

                // add collision points to array
                for (int i=0; i <collision_data.nb_collision_points; i++) {
                    if (nb_collision_points == MAX_COLLISION_POINTS) {break;}
                    collision_points[nb_collision_points] = collision_data.collision_points[i];
                    nb_collision_points++;
                }

                CollisionResolver::resolve(collision_data, delta_t);
            }
        }
    }

    double World::get_friction() const {
        return friction;
    }

    void World::set_friction(double f) {
        if (f < 0 || f > 1) {throw SimulationException("The friction must be a value between 0 & 1");}
        friction = f;
    }
} // Msfl2D