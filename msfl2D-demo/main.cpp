//
// Created by myselfleo on 27/06/2023.
//

#include <iostream>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "msfl2D/World.hpp"
#include "Interface.hpp"
#include "msfl2D/CollisionDetector.hpp"

using namespace Msfl2D;
using namespace Msfl2Demo;

/**
 * Print the last SDL error to the error output and exit the program with EXIT_FAILURE as its exit code.
 */
void sdl_failure() {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
}


/** Initialise SDL */
void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) sdl_failure();

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
            for (int i=0; i<result.nb_collision_points; i++) {
                interface.draw_point(result.collision_point[i], 3, Interface::COLOR_RED);
            }

            interface.draw_segment(result.reference_side);

            std::tuple<Vec2D, Vec2D> end_points = result.reference_side.coordinates();
            std::cout << std::get<0>(end_points) << "  " << std::get<1>(end_points) << std::endl;
            interface.draw_line(Line::from_director_vector(
                    std::get<0>(end_points),
                    result.minimum_penetration_vector
                    ));

            interface.draw_line(Line::from_director_vector(
                    std::get<1>(end_points),
                    result.minimum_penetration_vector
                    ));
        }


        /*
        result = CollisionDetector::sat(shape_3, shape_4);
        if (result.collide) {
            for (int i=0; i<result.nb_collision_points; i++) {
                interface.draw_point(result.collision_point[i], 3, Interface::COLOR_RED);
            }

            interface.draw_segment(result.reference_side);

            std::tuple<Vec2D, Vec2D> end_points = result.reference_side.coordinates();
            std::cout << std::get<0>(end_points) << "  " << std::get<1>(end_points) << std::endl;
            interface.draw_line(Line::from_director_vector(
                    std::get<0>(end_points),
                    result.minimum_penetration_vector
                    ));

            interface.draw_line(Line::from_director_vector(
                    std::get<1>(end_points),
                    result.minimum_penetration_vector
                    ));
        }*/


        /**
        Vec2D dir_vec_1 = {0, 1};
        Vec2D dir_vec_2 = {cos(ImGui::GetTime()), sin(ImGui::GetTime())};

        Line line1 = Line::from_director_vector({0, 1}, dir_vec_1);
        Line line2 = Line::from_director_vector({3, 2}, dir_vec_2);
        interface.draw_line(line1, Interface::COLOR_GREEN);
        interface.draw_line(line2, Interface::COLOR_GREEN);

        if (!Vec2D::collinear(dir_vec_1, dir_vec_2))
        interface.draw_point(Line::intersection(line1, line2), 10, Interface::COLOR_YELLOW);
        */

        interface.update_screen();
    }

    // Most of the cleanup is done by the Interface via reset()
    interface.reset();
    SDL_Quit();
    return EXIT_SUCCESS;
}