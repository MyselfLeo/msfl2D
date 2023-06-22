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
int WINDOW_SIZE[2] = {1280, 720};
const char * WINDOW_TITLE = "Msfl2D Demo";

// Camera related values
int CAMERA_SCREEN_POS[2] = {WINDOW_SIZE[0]/2, WINDOW_SIZE[1]/2};
const double CAMERA_ZOOM_LVL = 20;

// Rendering related values
const Uint8 BACKGROUND_COLOR[4] = {0, 0, 0, 255};
const Uint8 BACKGROUND_ACCENT_COLOR[4] = {50, 50, 50, 255};
const Uint8 SHAPE_COLOR[4] = {255, 255, 255, 255};

// UI related values
ConvexPolygon* hovered_shape = nullptr;

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
    res.y *= -1;
    res += Vec2D(CAMERA_SCREEN_POS[0], CAMERA_SCREEN_POS[1]);
    return res;
}

/**
 * Compute the world-space coordinates of a point in the screen space.

 */
Vec2D screen_to_world(const Vec2D& coordinates) {
    Vec2D res = coordinates - Vec2D(CAMERA_SCREEN_POS[0], CAMERA_SCREEN_POS[1]);
    res.y *= -1;
    res /= CAMERA_ZOOM_LVL;
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


void render_draw_convex_polygon_fill(SDL_Renderer * renderer, const ConvexPolygon& poly) {
    SDL_Color vertex_color = {SHAPE_COLOR[0], SHAPE_COLOR[1], SHAPE_COLOR[2], SHAPE_COLOR[3]};

    // convert Msfl2D polygon data into SDL geometry data
    std::vector<SDL_Vertex> sdl_vertices;

    Vec2D poly_center_pos = world_to_screen(poly.position);
    SDL_Vertex center_vertex = {
            {static_cast<float>(poly_center_pos.x), static_cast<float>(poly_center_pos.y)},
            {vertex_color},
            {1, 1}
    };


    for (int i=0; i<poly.vertices.size(); i++) {
        Vec2D current_v = poly.vertices[i];
        Vec2D next_v = poly.vertices[(i+1) % poly.vertices.size()];

        Vec2D screen_p = world_to_screen(current_v);
        sdl_vertices.push_back({
           {static_cast<float>(screen_p.x), static_cast<float>(screen_p.y)},
           vertex_color,
           {1, 1}
        });

        sdl_vertices.push_back(center_vertex);

        screen_p = world_to_screen(next_v);
        sdl_vertices.push_back({
           {static_cast<float>(screen_p.x), static_cast<float>(screen_p.y)},
           vertex_color,
           {1, 1}
        });
    }

    // Draw the polygon
    SDL_RenderGeometry(
            renderer,
            nullptr,
            sdl_vertices.data(),
            sdl_vertices.size(),
            nullptr,
            0);
    }



// Draw useful info in the background
void render_draw_background(SDL_Renderer * renderer) {
    int e = SDL_SetRenderDrawColor(
        renderer,
        BACKGROUND_ACCENT_COLOR[0],
        BACKGROUND_ACCENT_COLOR[1],
        BACKGROUND_ACCENT_COLOR[2],
        BACKGROUND_ACCENT_COLOR[3]);
    if (e < 0) sdl_failure();

    SDL_RenderDrawLine(renderer, CAMERA_SCREEN_POS[0], 0, CAMERA_SCREEN_POS[0], WINDOW_SIZE[1]);
    SDL_RenderDrawLine(renderer, 0, CAMERA_SCREEN_POS[1], WINDOW_SIZE[0], CAMERA_SCREEN_POS[1]);
}



Vec2D get_mouse_pos() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {x * 1.0, y * 1.0};
}




void render_window_main() {
    ImGui::Begin("General info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    Vec2D screen_mouse_pos = get_mouse_pos();
    Vec2D world_mouse_pos = screen_to_world(screen_mouse_pos);
    ImGui::Text("Screen mouse position: %.0f  %.0f", screen_mouse_pos.x, screen_mouse_pos.y);
    ImGui::Text("World mouse position: %.0f  %.0f", world_mouse_pos.x, world_mouse_pos.y);

    ImGui::End();
}

void render_window_hovered_shape() {
    ImGui::Begin("Hovered Shape", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (hovered_shape == nullptr) {
        ImGui::Text("Not hovering a shape");
    }
    else {
        ImGui::Text("Nb of vertices: %zu", hovered_shape->vertices.size());
        ImGui::Text("Position: %.0f  %.0f", hovered_shape->position.x, hovered_shape->position.y);
    }

    ImGui::End();
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







void render(SDL_Window * window, std::vector<ConvexPolygon>& polygons) {
    SDL_Renderer * renderer = SDL_GetRenderer(window);


    SDL_GetWindowSize(window, &WINDOW_SIZE[0], &WINDOW_SIZE[1]);
    CAMERA_SCREEN_POS[0] = WINDOW_SIZE[0] / 2;
    CAMERA_SCREEN_POS[1] = WINDOW_SIZE[1] / 2;

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



    // Background drawing
    render_draw_background(renderer);


    // Polygon drawing
    hovered_shape = nullptr;
    for (auto& p: polygons) {
        if (p.is_point_inside(screen_to_world(get_mouse_pos()))) {
            hovered_shape = &p;
            render_draw_convex_polygon_fill(renderer, p);
        }
        else {renderer_draw_convex_polygon(renderer, p);}
    }



    // Window drawing
    render_window_main();
    render_window_hovered_shape();



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

    // not a convexpolygon; should result in a crash
    //ConvexPolygon poly = ConvexPolygon({{1,1}, {1, -1}, {-1, -1}, {-1, 1}, {-2, 3}});

    std::vector<ConvexPolygon> polys = {
            ConvexPolygon({{1,1}, {1, -1}, {-1, -1}, {-1, 1}})
    };


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


        // Update screenpoly
        render(window, polys);

    }

    // SDL & ImGui cleanup
    cleanup(window);

    return EXIT_SUCCESS;
}