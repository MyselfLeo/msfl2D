//
// Created by myselfleo on 27/06/2023.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "imgui.h"
#include "msfl2D/World.hpp"
#include "Interface.hpp"
#include "msfl2D/CollisionDetector.hpp"

using namespace Msfl2D;
using namespace Msfl2Demo;



/** Initialise SDL */
void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) Interface::sdl_failure();
    if (TTF_Init() != 0) {Interface::sdl_ttf_failure();}
}




int main(int argc, char *argv[]) {
    init_sdl();

    std::shared_ptr<ConvexPolygon> shape_1 = std::make_shared<ConvexPolygon>(ConvexPolygon(3, 1, {5, 7}));
    std::shared_ptr<Body> body_1 = std::make_shared<Body>(Body());
    body_1->add_shape(shape_1);

    std::shared_ptr<ConvexPolygon> shape_2 = std::make_shared<ConvexPolygon>(ConvexPolygon(5, 2, {-1, -3}));
    std::shared_ptr<Body> body_2 = std::make_shared<Body>(Body());
    body_2->add_shape(shape_2);

    std::shared_ptr<ConvexPolygon> shape_3 = std::make_shared<ConvexPolygon>(ConvexPolygon(4, 1, {-3, -5}));
    std::shared_ptr<Body> body_3 = std::make_shared<Body>(Body());
    body_3->add_shape(shape_3);
    body_3->rotate(M_PI / 4);

    std::shared_ptr<ConvexPolygon> shape_4 = std::make_shared<ConvexPolygon>(ConvexPolygon(4, 2, {-3, -10}));
    std::shared_ptr<Body> body_4 = std::make_shared<Body>(Body());
    body_4->add_shape(shape_4);
    body_4->rotate(M_PI / 4);



    std::shared_ptr<World> world = std::make_shared<World>(World());
    world->add_body(body_1);
    world->add_body(body_2);
    world->add_body(body_3);
    world->add_body(body_4);

    Interface interface = Interface(world);

    interface.init_window({1280, 720}, "Msfl2D Demo");

    // Main loop
    bool stop = false;
    while (!stop) {
        // Process I/O events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            interface.process(&event);
            if (event.type == SDL_QUIT) {stop = true;} // end condition
        }

        interface.process_io();
        interface.update();

        interface.render(false);

        SATResult result = CollisionDetector::sat(shape_1, shape_2);
        if (result.collide) {

            auto l1 = Line::from_director_vector(std::get<0>(result.reference_side.coordinates()), result.minimum_penetration_vector);
            auto l2 = Line::from_director_vector(std::get<1>(result.reference_side.coordinates()), result.minimum_penetration_vector);


            interface.draw_segment(result.reference_side);
            interface.draw_text(std::to_string(result.depth).c_str(), {0, 0});
            interface.draw_line(l1);
            interface.draw_line(l2);


            for (int i=0; i<result.nb_collision_points; i++) {
                interface.draw_point(result.collision_points[i], 5, Interface::COLOR_RED);
            }
        }

        result = CollisionDetector::sat(shape_3, shape_4);
        if (result.collide) {

            auto l1 = Line::from_director_vector(std::get<0>(result.reference_side.coordinates()), result.minimum_penetration_vector);
            auto l2 = Line::from_director_vector(std::get<1>(result.reference_side.coordinates()), result.minimum_penetration_vector);


            interface.draw_segment(result.reference_side);
            interface.draw_line(l1);
            interface.draw_line(l2);


            for (int i=0; i<result.nb_collision_points; i++) {
                interface.draw_point(result.collision_points[i], 5, Interface::COLOR_RED);
            }
        }




        interface.update_screen();
    }

    // Most of the cleanup is done by the Interface via reset()
    interface.reset();
    SDL_Quit();
    return EXIT_SUCCESS;
}