//
// Created by leo on 12/06/23.
//
#include <iostream>

#include <SDL2/SDL.h>

#include "msfl2D/Vec2D.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "ConvexPolygon.hpp"

using namespace Msfl2D;



const int WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
const int WINDOW_SIZE[2] = {1280, 720};
const char * WINDOW_TITLE = "Msfl2D Demo";

// Camera related values
const int CAMERA_SCREEN_POS[2] = {WINDOW_SIZE[0]/2, WINDOW_SIZE[1]/2};
const double CAMERA_ZOOM_LVL = 20;

// Rendering related values
const int BACKGROUND_COLOR[4] = {0, 0, 0, 255};
const int SHAPE_COLOR[4] = {255, 255, 255, 255};

/**
 * Print the last SDL error to the error output and exit the program with EXIT_FAILURE as its exit code.
 */
void sdl_failure() {
    std::cerr << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
}



/**
 * Compute the screen-space coordinates of a point in the World space.
 */
Vec2D world_to_screen(const Vec2D& coordinates) {
    Vec2D res = coordinates * CAMERA_ZOOM_LVL;
    res.y *= 1;
    res += Vec2D(CAMERA_SCREEN_POS[0], CAMERA_SCREEN_POS[1]);
    return res;
}







void renderer_draw_convex_polygon(SDL_Renderer * renderer, const ConvexPolygon& poly) {
    int e = SDL_SetRenderDrawColor(
        renderer,
        SHAPE_COLOR[0],
        SHAPE_COLOR[1],
        SHAPE_COLOR[2],
        SHAPE_COLOR[3]);
    if (e < 0) sdl_failure();

    for (int i=0; i<poly.vertices.size(); i++) {
        Vec2D screen_p_1 = world_to_screen(poly.vertices[i]);
        Vec2D screen_p_2 = world_to_screen(poly.vertices[(i+1) % poly.vertices.size()]);

        e = SDL_RenderDrawLine(renderer, screen_p_1.x, screen_p_1.y, screen_p_2.x, screen_p_2.y);
        if (e < 0) sdl_failure();
    }
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







void render(SDL_Window * window, const ConvexPolygon& p) {
    SDL_Renderer * renderer = SDL_GetRenderer(window);

    // Clearing the last frame
    SDL_SetRenderDrawColor(
            renderer,
            BACKGROUND_COLOR[0],
            BACKGROUND_COLOR[1],
            BACKGROUND_COLOR[2],
            BACKGROUND_COLOR[3]);

    SDL_RenderClear(renderer);

    // new frame initialisation
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();



    // drawing happens here
    renderer_draw_convex_polygon(renderer, p);
    ImGui::ShowDemoWindow();



    // frame rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);

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


    ConvexPolygon poly = ConvexPolygon({{1,1}, {1, -1}, {-1, -1}, {-1, 1}});


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
        render(window, poly);

    }

    // SDL & ImGui cleanup
    cleanup(window);

    return EXIT_SUCCESS;
}