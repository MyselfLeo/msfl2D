//
// Created by leo on 12/06/23.
//
#include <iostream>

#include <SDL2/SDL.h>

#include "msfl2D/Vec2D.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

using namespace Msfl2D;



const int WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
const int WINDOW_SIZE[2] = {1280, 720};
const int BACKGROUND_COLOR[3] = {0, 0, 0};
const char * WINDOW_TITLE = "Msfl2D Demo";



/**
 * Print the last SDL error to the error output and exit the program with EXIT_FAILURE as its exit code.
 */
void sdl_failure() {
    std::cerr << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
}


/**
 * Initialise SDL the SDL renderer and ImGui, open and return a window (with a renderer already created for it).
 * May crash the program in case of an SDL or ImGui error.
 */
SDL_Window * init_renderer() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) sdl_failure();

    SDL_Window * window = SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_SIZE[0],
            WINDOW_SIZE[1],
            WINDOW_FLAGS);
    if (window == nullptr) sdl_failure();

    SDL_Renderer * renderer = SDL_CreateRenderer(
            window,
            -1,
            RENDERER_FLAGS);
    if (renderer == nullptr) sdl_failure();


    // Initialise ImGui and the SDL_Renderer backend
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return window;
}





void process_event(SDL_Event& event) {
    //todo
}







void render(SDL_Window * window) {
    SDL_Renderer * renderer = SDL_GetRenderer(window);

    // Clearing the last frame
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], 255);
    SDL_RenderClear(renderer);

    // new frame initialisation
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();



    // drawing happens here
    ImGui::ShowDemoWindow();



    // frame rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(SDL_GetRenderer(window));

}





void cleanup(SDL_Window * window) {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(SDL_GetRenderer(window));
    SDL_DestroyWindow(window);
    SDL_Quit();
}




int main() {

    // Create a window
    SDL_Window * window = init_renderer();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Main event loop
    bool stop = false;
    while (!stop) {

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // ImGui has priority over the rest of the program on the events.
            // events treated by ImGui are discarded.
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (!io.WantCaptureMouse) {
                process_event(event);
            }

            // Exit event
            if (event.type == SDL_QUIT) {
                stop = true;
            }
        }


        // Update screen
        render(window);
    }

    // SDL & ImGui cleanup
    cleanup(window);

    return EXIT_SUCCESS;
}