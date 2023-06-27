//
// Created by myselfleo on 27/06/2023.
//

#include "WorldRenderer.hpp"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <utility>

namespace Msfl2Demo {

    const Color4 WorldRenderer::BACKGROUND_COLOR = {0, 0, 0};
    const Color4 WorldRenderer::BACKGROUND_INFO_COLOR = {50, 50, 50};
    const Color4 WorldRenderer::MAIN_COLOR = {255, 255, 255};
    const Color4 WorldRenderer::COLOR_RED = {255, 0, 0};
    const Color4 WorldRenderer::COLOR_GREEN = {0, 255, 0};
    const Color4 WorldRenderer::COLOR_BLUE = {0, 0, 255};
    const Color4 WorldRenderer::COLOR_YELLOW = {255, 255, 0};


    int WorldRenderer::NB_CREATED = 0;

    WorldRenderer::WorldRenderer(std::shared_ptr<Msfl2D::World> world): world(std::move(world)) {
        // Only one WorldRenderer can exist at a time; we check that.
        if (NB_CREATED > 0) {
            std::cerr << "Tried to create a WorldRenderer while one still exists" << std::endl;
            exit(EXIT_FAILURE);
        }

        NB_CREATED += 1;

        renderer = nullptr;
        window = nullptr;

        camera_pos = Msfl2D::Vec2D(0, 0);
    }

    WorldRenderer::~WorldRenderer() {
        if (renderer != nullptr || window != nullptr) {reset();}
        NB_CREATED -= 1;
    }

    void WorldRenderer::init_window(const Msfl2D::Vec2D &size, const std::string &name) {
        if (window != nullptr || renderer != nullptr) {
            std::cerr << "Cannot instantiate 2 window for one renderer" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (size.x < 1 || size.y < 1) {
            std::cerr << "The size of the window must be positive" << std::endl;
            exit(EXIT_FAILURE);
        }

        window_name = name;
        window_size = size;

        // Create SDL Window & Renderer
        window = SDL_CreateWindow(
                name.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_size.x,
                window_size.y,
                WINDOW_FLAGS
                );
        if (window == nullptr) {
            std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(
                window,
                -1,
                RENDERER_FLAGS
                );
        if (renderer == nullptr) {
            std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        // Initialise ImGui and the SDL_Renderer backend
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }


    void WorldRenderer::reset() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        // Should not be a problem if renderer or window is nullptr
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        renderer = nullptr;
        window = nullptr;

    }



    void WorldRenderer::process(const SDL_Event *event) {
        // ImGui has priority over the rest of the program on the events.
        // events treated by ImGui are discarded, so we return early.
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplSDL2_ProcessEvent(event);
        if (io.WantCaptureMouse) {return;}

        // Now we can process the event as it was not consumed by ImGui.

    }



    void WorldRenderer::process_io() {
        const Uint8* keys_state = SDL_GetKeyboardState(nullptr);

        if (keys_state[SDL_SCANCODE_UP]) {camera_pos.y += CAMERA_SPEED;}
        if (keys_state[SDL_SCANCODE_DOWN]) {camera_pos.y -= CAMERA_SPEED;}
        if (keys_state[SDL_SCANCODE_RIGHT]) {camera_pos.x += CAMERA_SPEED;}
        if (keys_state[SDL_SCANCODE_LEFT]) {camera_pos.x -= CAMERA_SPEED;}
    }



    void WorldRenderer::render() {
        if (renderer == nullptr) {
            std::cerr << "Called render on an uninitialised WorldRenderer" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Clear the screen
        set_color(BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        // ImGui new frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();



        // World drawing
        draw_background();



        ImGui::Begin("hello", nullptr, IMGUI_WINDOW_FLAGS);
        ImGui::Text("hello");
        ImGui::End();


        // frame rendering
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }


    void WorldRenderer::set_color(Color4 color) const {
        if (renderer == nullptr) {
            std::cerr << "Called set_color on an uninitialised WorldRenderer" << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }



    Msfl2D::Vec2D WorldRenderer::world_to_screen(const Msfl2D::Vec2D &coo) const {
        Msfl2D::Vec2D res = coo - camera_pos;
        res *= camera_zoom_lvl;
        res.y *= -1;               // screen y is top to bottom while we want bottom to top.
        res += Msfl2D::Vec2D(window_size.x, window_size.y) / 2;
        return res;
    }

    Msfl2D::Vec2D WorldRenderer::screen_to_world(const Msfl2D::Vec2D &coo) const {
        Msfl2D::Vec2D res = coo - Msfl2D::Vec2D(window_size.x, window_size.y) / 2;
        res.y *= -1;               // See world_to_screen()
        res /= camera_zoom_lvl;
        res += camera_pos;
        return res;
    }


    Msfl2D::Vec2D WorldRenderer::get_mouse_pos() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return {x * 1.0, y * 1.0};
    }

    void WorldRenderer::draw_background(const Color4 &color) const {
        // Draw the 2 axis
        draw_line(Line::from_director_vector({0, 0}, {0, 1}), color);
        draw_line(Line::from_director_vector({0, 0}, {1, 0}), color);
    }

    void WorldRenderer::draw_line(const Line &line, const Color4 &color) const {
        set_color(color);

        // End points of the line to draw on the screen
        Vec2D p1, p2;

        // If the line is vertical, it's easier to draw, so we use a special case for that
        if (line.is_vertical()) {
            p1 = world_to_screen({line.find_x(0), 0});
            p2 = p1;
            p1.y = 0;
            p2.y = window_size.y;
        }
        else {
            // This is a bit more touchy:
            // We compute a screen-space line, then we either clamp its x or y coordinates to the one of the screen
            // (depending on the slope, so the actually drawn line does not overflow too much from the window)
            // (idk if it's really useful, but oh well)
            double slope = line.get_slope() * -1;
            Vec2D line_p = world_to_screen({0, line.get_zero()});

            // screen bounds
            std::tuple<Vec2D, Vec2D> sb = get_screen_bounds();

            // clamp on y
            if (abs(slope) > 1) {
                // this is wonky, but well then
                p1 = world_to_screen( {line.find_x(std::get<0>(sb).y), std::get<0>(sb).y});
                p2 = world_to_screen( {line.find_x(std::get<1>(sb).y), std::get<1>(sb).y});
            }
            // clamp on x
            else {
                p1 = world_to_screen({std::get<0>(sb).x, line.find_y(std::get<0>(sb).x)});
                p2 =  world_to_screen({std::get<1>(sb).x, line.find_y(std::get<1>(sb).x)});
            }
        }

        //std::cout << p1 << "   " << p2 << std::endl;

        // Draw the line
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }

    void WorldRenderer::draw_segment(const Segment &segment, const Line &line, const Color4 &color) const {
        set_color(color);
    }

    void WorldRenderer::draw_point(const Vec2D &point, const Color4 &color) {
        set_color(color);
    }

    std::tuple<Vec2D, Vec2D> WorldRenderer::get_screen_bounds() const {
        return {screen_to_world({0, 0}), screen_to_world(window_size - Vec2D(1,1))};
    }
} // Msfl2Demo
