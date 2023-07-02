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

    std::shared_ptr<ConvexPolygon> shape_1;
    shape_1.reset(new ConvexPolygon(3, 1, {5, 7}));

    std::shared_ptr<ConvexPolygon> shape_2;
    shape_2.reset(new ConvexPolygon(5, 2, {-1, -3}));

    std::shared_ptr<Body> body_1 = std::make_shared<Body>(Body());
    body_1->add_shape(shape_1);

    std::shared_ptr<Body> body_2 = std::make_shared<Body>(Body());
    body_2->add_shape(shape_2);

    std::shared_ptr<World> world = std::make_shared<World>(World());
    world->add_body(body_1);
    world->add_body(body_2);

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
            Line line = Line::from_director_vector({0, 0}, result.minimum_penetration_vector);
            interface.draw_line(line, Interface::COLOR_YELLOW);
        }

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