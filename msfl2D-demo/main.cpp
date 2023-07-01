//
// Created by myselfleo on 27/06/2023.
//

#include <iostream>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "msfl2D/World.hpp"
#include "Interface.hpp"

using namespace Msfl2D;

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

    std::shared_ptr<Shape> triangle_1;
    triangle_1.reset(new ConvexPolygon(3, 1, {5, 7}));

    std::shared_ptr<Shape> triangle_2;
    triangle_2.reset(new ConvexPolygon(3, 1, {-1, -3}));

    std::shared_ptr<Body> body = std::make_shared<Body>(Body());
    body->add_shape(triangle_1)
         .add_shape(triangle_2);

    std::shared_ptr<World> world = std::make_shared<World>(World());
    world->add_body(body);

    Msfl2Demo::Interface interface = Msfl2Demo::Interface(world);

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

        body->rotate(0.01, {0, 0});
        body->rotate(-0.03);
        body->rotate_shape(1, body->get_shape(1)->get_rotation() + 0.04);
        body->rotate_shape(0, body->get_shape(0)->get_rotation() - 0.06);

        interface.render();
    }

    // Most of the cleanup is done by the Interface via reset()
    interface.reset();
    SDL_Quit();
    return EXIT_SUCCESS;
}