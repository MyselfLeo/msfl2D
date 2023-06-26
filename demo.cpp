//
// Created by myselfleo on 12/06/23.
//

// This file is huge and unreadable. This is just a demo.

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "Vec2D.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "ConvexPolygon.hpp"
#include "Segment.hpp"

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
const Uint8 SEGMENT_COLOR[4] = {255, 0, 0, 255};

// UI related values
ConvexPolygon* hovered_shape = nullptr;
ConvexPolygon* selected_shape = nullptr;
int selection_offset[2] = {0, 0};

// DEBUG
double PROJECTED_POINT[2] = {0, 0};

/**
 * Print the last SDL error to the error output and exit the program with EXIT_FAILURE as its exit code.
 */
void sdl_failure() {
    std::cerr << "SDL Error:" << std::endl;
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
        Vec2D screen_p_1 = world_to_screen(poly.vertices[i] + poly.position);
        Vec2D screen_p_2 = world_to_screen(poly.vertices[(i+1) % poly.vertices.size()] + poly.position);

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
        Vec2D current_v = poly.vertices[i] + poly.position;
        Vec2D next_v = poly.vertices[(i+1) % poly.vertices.size()] + poly.position;

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



// Draw a line on the screen
void render_draw_line(SDL_Renderer * renderer, const Line& line) {
    int e = SDL_SetRenderDrawColor(
        renderer,
        SHAPE_COLOR[0],
        SHAPE_COLOR[1],
        SHAPE_COLOR[2],
        SHAPE_COLOR[3]);
    if (e < 0) sdl_failure();

    Vec2D p1;
    Vec2D p2;

    if (line.is_vertical()) {
        p1 = world_to_screen({line.find_x(0), 0});
        p1.y = 0;
        p2 = world_to_screen({line.find_x(0), 0});
        p2.y = WINDOW_SIZE[1];
    }
    else {
        double slope = line.get_slope() * -1;
        Vec2D passage_point = world_to_screen({0, line.get_zero()});

        // Create a screen-space line
        Line screen_space_line = Line(passage_point, passage_point + Vec2D(1, slope));

        // Get the points at x = 0 and x = WINDOW_SIZE[0].
        p1 = {0, screen_space_line.find_y(0)};
        p2 = {static_cast<double>(WINDOW_SIZE[0]), screen_space_line.find_y(WINDOW_SIZE[0])};
    }

    // now, we just draw the line
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}



void render_draw_segment(SDL_Renderer * renderer, const Segment& segment, const Line& line) {
    int e = SDL_SetRenderDrawColor(
        renderer,
        SEGMENT_COLOR[0],
        SEGMENT_COLOR[1],
        SEGMENT_COLOR[2],
        SEGMENT_COLOR[3]);
    if (e < 0) sdl_failure();

    std::tuple<Vec2D, Vec2D> points = segment.coordinates(line);

    // convert world coordinates to screen coordinates
    Vec2D p1 = world_to_screen(std::get<0>(points));
    Vec2D p2 = world_to_screen(std::get<01>(points));

    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderDrawLine(renderer, p1.x-1, p1.y-1, p2.x-1, p2.y-1); // double it and give it to the next person
}



void render_draw_point(SDL_Renderer * renderer, const Vec2D& point, int size) {
    if (size % 2 == 0) {size += 1;}

    int e = SDL_SetRenderDrawColor(
        renderer,
        SEGMENT_COLOR[0],
        SEGMENT_COLOR[1],
        SEGMENT_COLOR[2],
        SEGMENT_COLOR[3]);
    if (e < 0) sdl_failure();


    Vec2D screen_coo = world_to_screen(point);

    SDL_Rect r;
    r.x = screen_coo.x - ((size-1)/2);
    r.y = screen_coo.y - ((size-1)/2);
    r.w = size;
    r.h = size;
    SDL_RenderFillRect(renderer, &r);
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



void unique_input_process() {
    const Uint8* keys_state = SDL_GetKeyboardState(nullptr);

    if (keys_state[SDL_SCANCODE_UP]) {PROJECTED_POINT[1] += 0.1;}
    if (keys_state[SDL_SCANCODE_DOWN]) {PROJECTED_POINT[1] -= 0.1;}
    if (keys_state[SDL_SCANCODE_RIGHT]) {PROJECTED_POINT[0] += 0.1;}
    if (keys_state[SDL_SCANCODE_LEFT]) {PROJECTED_POINT[0] -= 0.1;}
}



void process_event(SDL_Event& event, std::vector<ConvexPolygon>& polygons) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: {
            if (selected_shape != nullptr) {selected_shape = nullptr;}
            else {
                for (auto& p: polygons) {
                    if (p.is_point_inside(screen_to_world(get_mouse_pos()))) {
                        selected_shape = &p;
                        Vec2D offset = world_to_screen(p.position) - get_mouse_pos();
                        selection_offset[0] = offset.x;
                        selection_offset[1] = offset.y;
                        break;
                    }
                }
            }
        } break;

        case SDL_MOUSEBUTTONUP: {
            if (selected_shape == nullptr) {break;}
            else {selected_shape = nullptr;}
        } break;
    }
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




    double t = ImGui::GetTime();
    Vec2D dir_vec = {cos(t), sin(t)};
    //Vec2D dir_vec = {1, 0};
    Line line = Line::from_director_vector({0, 0}, dir_vec);
    render_draw_line(renderer, line);

    render_draw_point(renderer, line.get_origin(), 6);
    render_draw_point(renderer, line.get_coo_grad(1), 4);

    render_draw_point(renderer, Vec2D(PROJECTED_POINT[0], PROJECTED_POINT[1]), 5);

    Vec2D proj = line.get_coo_grad(Vec2D(PROJECTED_POINT[0], PROJECTED_POINT[1]).project(line));

    render_draw_point(renderer, proj, 5);


    // Projected segments
    for (auto& p: polygons) {
        Segment projection = p.project(line);
        render_draw_segment(renderer, projection, line);
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




void update() {
    if (selected_shape != nullptr) {
        selected_shape->position = screen_to_world(get_mouse_pos() + Vec2D(selection_offset[0], selection_offset[1]));
    }
}









int main(int argc, char *argv[]) {
    // Create a window
    SDL_Window *window = init_renderer();

    ImGuiIO &io = ImGui::GetIO(); (void) io;

    std::vector<ConvexPolygon> polys = {
            ConvexPolygon({
                            {1,  1},
                           {1,  -1},
                           {-1, -1},
                           {-1, 1}}, {5, 5}),

            ConvexPolygon({{1,   1},
                           {1.5, 0},
                           {1,   -1},
                           {-1,  -1},
                           {-1,  1}}, {-10, 10}),

            // Concave polygon; adding it will lead to a crash at startup
            /*ConvexPolygon({{1,  1},
                           {1,  -1},
                           {-1, -1},
                           {-1, 1},
                           {-2, 3}}, {-2, -3})*/
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
                process_event(event, polys);
            }

            // Exit event
            if (event.type == SDL_QUIT) {
                stop = true;
            }
        }
        unique_input_process();

        update();

        // Update screen
        render(window, polys);

    }

    // SDL & ImGui cleanup
    cleanup(window);

    return EXIT_SUCCESS;
}