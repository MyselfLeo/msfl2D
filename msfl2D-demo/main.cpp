//
// Created by myselfleo on 27/06/2023.
//

#include <iostream>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "msfl2D/World.hpp"
#include "WorldRenderer.hpp"

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
    
    // Empty world; for now
    std::shared_ptr<World> world = std::make_shared<World>(World());
    Msfl2Demo::WorldRenderer world_renderer = Msfl2Demo::WorldRenderer(world);

    world_renderer.init_window({1280, 720}, "Msfl2D Demo");

    // Main loop
    bool stop = false;
    while (!stop) {
        // Process I/O events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            //todo: WorldRenderer should grab the event here and process it

            // end condition
            if (event.type == SDL_QUIT) {stop = true;}
        }

        world_renderer.render();
    }

    // Most of the cleanup is done by the WorldRenderer via reset()
    world_renderer.reset();
    SDL_Quit();
    return EXIT_SUCCESS;
}