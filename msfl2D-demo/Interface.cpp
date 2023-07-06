//
// Created by myselfleo on 27/06/2023.
//

#include "Interface.hpp"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <utility>

namespace Msfl2Demo {

    void Interface::sdl_failure()  {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    void Interface::sdl_ttf_failure() {
        std::cerr << "SDL_TTF Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }


    const Color4 Interface::BACKGROUND_COLOR = {0, 0, 0};
    const Color4 Interface::BACKGROUND_INFO_COLOR = {50, 50, 50};
    const Color4 Interface::SHAPE_OUTLINE_COLOR = {255, 255, 255};
    const Color4 Interface::SHAPE_AREA_COLOR = {255, 255, 255, 155};
    const Color4 Interface::COLOR_RED = {255, 0, 0};
    const Color4 Interface::COLOR_GREEN = {0, 255, 0};
    const Color4 Interface::COLOR_BLUE = {3, 123, 252};
    const Color4 Interface::COLOR_YELLOW = {255, 255, 0};


    int Interface::NB_CREATED = 0;

    Interface::Interface(std::shared_ptr<Msfl2D::World> world): world(std::move(world)) {
        // Only one Interface can exist at a time; we check that.
        if (NB_CREATED > 0) {
            std::cerr << "Tried to create a Interface while one still exists" << std::endl;
            exit(EXIT_FAILURE);
        }

        NB_CREATED += 1;

        renderer = nullptr;
        window = nullptr;

        camera_pos = Msfl2D::Vec2D(0, 0);

        // initialise the font
        font = TTF_OpenFont("data/ConsolaMono-Book.ttf", 12);
        if (font == nullptr) {sdl_ttf_failure();}
    }

    Interface::~Interface() {
        if (renderer != nullptr || window != nullptr) {reset();}
        NB_CREATED -= 1;
    }

    void Interface::init_window(const Msfl2D::Vec2D &size, const std::string &name) {
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
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Initialise ImGui and the SDL_Renderer backend
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }


    void Interface::reset() {
        TTF_CloseFont(font);

        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        // Should not be a problem if renderer or window is nullptr
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        renderer = nullptr;
        window = nullptr;

    }



    void Interface::process(const SDL_Event *event) {
        // ImGui has priority over the rest of the program on the events.
        // events treated by ImGui are discarded, so we return early.
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplSDL2_ProcessEvent(event);
        if (io.WantCaptureMouse) {return;}

        // Now we can process the event as it was not consumed by ImGui.
        switch (event->type) {
            case SDL_MOUSEBUTTONDOWN: {
                if (selected_body == nullptr) {
                    // Find if a body is hovered. If so, grab it.
                    for (auto& b: world->get_bodies()) {
                        if (is_body_hovered(b.second)) {
                            selected_body = b.second;
                            selection_pixel_offset = world_to_screen(b.second->position) - get_mouse_pos();
                            break;
                        }
                    }
                }
            } break;

            case SDL_MOUSEBUTTONUP: {
                if (selected_body != nullptr) {
                    selected_body = nullptr;
                }
            } break;

            case SDL_MOUSEWHEEL: {
                camera_zoom_lvl *= pow(1.10, event->wheel.y);
                if (camera_zoom_lvl < 1) {camera_zoom_lvl = 1;}
            } break;
        }
    }



    void Interface::process_io() {
        const Uint8* keys_state = SDL_GetKeyboardState(nullptr);

        double rel_camera_speed = 10 * CAMERA_SPEED / camera_zoom_lvl;
        if (keys_state[SDL_SCANCODE_UP] || keys_state[SDL_SCANCODE_W]) {camera_pos.y += rel_camera_speed;}
        if (keys_state[SDL_SCANCODE_DOWN] || keys_state[SDL_SCANCODE_S]) {camera_pos.y -= rel_camera_speed;}
        if (keys_state[SDL_SCANCODE_RIGHT] || keys_state[SDL_SCANCODE_D]) {camera_pos.x += rel_camera_speed;}
        if (keys_state[SDL_SCANCODE_LEFT] || keys_state[SDL_SCANCODE_A]) {camera_pos.x -= rel_camera_speed;}

        if (keys_state[SDL_SCANCODE_PAGEUP]) {camera_zoom_lvl *= 1.02;}
        if (keys_state[SDL_SCANCODE_PAGEDOWN]) {camera_zoom_lvl *= 0.98;}

        if (camera_zoom_lvl < 1) {camera_zoom_lvl = 1;}
    }



    void Interface::render(bool update) {
        if (renderer == nullptr) {
            std::cerr << "Called render on an uninitialised Interface" << std::endl;
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

        for (auto& b: world->get_bodies()) {
            draw_body(b);
        }



        // Window drawing
        create_camera_window();
        create_debug_tools_window();
        create_world_info_window();


        // frame rendering
        if (update) {update_screen();}
    }

    void Interface::update_screen() {
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }


    void Interface::set_color(Color4 color) const {
        if (renderer == nullptr) {
            std::cerr << "Called set_color on an uninitialised Interface" << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }




    Msfl2D::Vec2D Interface::world_to_screen(const Msfl2D::Vec2D &coo) const {
        Msfl2D::Vec2D res = coo - camera_pos;
        res *= camera_zoom_lvl;
        res.y *= -1;               // screen y is top to bottom while we want bottom to top.
        res += Msfl2D::Vec2D(window_size.x, window_size.y) / 2;
        return res;
    }

    Msfl2D::Vec2D Interface::screen_to_world(const Msfl2D::Vec2D &coo) const {
        Msfl2D::Vec2D res = coo - Msfl2D::Vec2D(window_size.x, window_size.y) / 2;
        res.y *= -1;               // See world_to_screen()
        res /= camera_zoom_lvl;
        res += camera_pos;
        return res;
    }


    Msfl2D::Vec2D Interface::get_mouse_pos() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return {x * 1.0, y * 1.0};
    }

    void Interface::draw_background(const Color4 &color) const {
        // Draw the 2 axis
        draw_line(Line::from_director_vector({0, 0}, {0, 1}), color);
        draw_line(Line::from_director_vector({0, 0}, {1, 0}), color);

        // Draw points some points along the axis to get a grasp of the size
        for (int i=1; i<4; i++) {
            draw_point({0, static_cast<double>(i)}, 3, color);
            draw_point({static_cast<double>(i), 0}, 3, color);
        }
    }

    void Interface::draw_line(const Line &line, const Color4 &color) const {
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

        // Draw the line
        int r = SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
        if (r != 0) {sdl_failure();}
    }

    void Interface::draw_segment(const LineSegment& segment, const Color4 &color) const {
        set_color(color);

        std::tuple<Vec2D, Vec2D> points = segment.coordinates();
        // convert world coordinates to screen coordinates
        Vec2D p1 = world_to_screen(std::get<0>(points));
        Vec2D p2 = world_to_screen(std::get<1>(points));

        // draw multiple lines to make the line fatter
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                int r = SDL_RenderDrawLine(renderer, p1.x + x, p1.y + y, p2.x + x, p2.y + y);
                if (r != 0) {sdl_failure();}
            }
        }

        draw_point(std::get<0>(points), 9, COLOR_BLUE);
        draw_text(std::to_string(segment.segment.min).c_str(), std::get<0>(points), COLOR_BLUE);
        draw_point(std::get<1>(points), 5, COLOR_BLUE);
        draw_text(std::to_string(segment.segment.max).c_str(), std::get<1>(points), COLOR_BLUE);
    }

    void Interface::draw_point(const Vec2D &point, int size, const Color4 &color) const {
        set_color(color);

        // odd size required to have symmetrical point
        if (size % 2 == 0) {size += 1;}

        Vec2D screen_coo = world_to_screen(point);

        SDL_Rect r = {
                int(screen_coo.x - int((size-1)/2)),
                int(screen_coo.y - int((size-1)/2)),
                size,
                size
        };

        int e = SDL_RenderFillRect(renderer, &r);
        if (e != 0) {sdl_failure();}
    }


    void Interface::draw_body(const std::pair<BodyID, std::shared_ptr<Msfl2D::Body>>& body_data) const {
        BodyID body_id = body_data.first;
        std::shared_ptr<Msfl2D::Body> body = body_data.second;
        bool hovered = is_body_hovered(body);
        for (auto& s: body->get_shapes()) {
            // Choose the correct drawing method based on the shape and if it is hovered or not
            if (hovered) {
                auto as_convex = std::dynamic_pointer_cast<ConvexPolygon>(s);
                if (as_convex != nullptr) {
                    draw_polygon_filled(*as_convex);
                    if (debug_centers) {draw_point(as_convex->get_position());}
                    continue;
                }
            }
            else {
                auto as_convex = std::dynamic_pointer_cast<ConvexPolygon>(s);
                if (as_convex != nullptr) {
                    draw_polygon_outline(*as_convex);
                    if (debug_centers) {draw_point(as_convex->get_position());}
                    continue;
                }
            }
            // Unknown type so exit program
            // (on drawing success, the loop should have continued by now)
            std::cerr << "World contains an unknown/undrawable shape type" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (debug_centers) { draw_point(body->position, 3, COLOR_GREEN);}
        if (debug_bodyids) { draw_text(std::to_string(body_id).c_str(), body->position, COLOR_GREEN);}
    }


    void Interface::draw_polygon_outline(const ConvexPolygon &p, const Color4 &color) const {
        set_color(color);

        for (int i=0; i<p.nb_vertices(); i++) {
            Vec2D p1 = world_to_screen(p.get_global_vertex(i));
            Vec2D p2 = world_to_screen(p.get_global_vertex((i+1) % p.nb_vertices()));

            int r = SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
            if (r != 0) {sdl_failure();}
        }
    }


    void Interface::draw_polygon_filled(const ConvexPolygon &p, const Color4 &color) const {
        draw_polygon_outline(p);

        // This function uses SDL_RenderGeometry to draw the filled polygon.
        set_color(color);

        Vec2D p_center = world_to_screen(p.get_position());
        SDL_Vertex center_vertex = {
                static_cast<float>(p_center.x),
                static_cast<float>(p_center.y),
                (SDL_Color) color,
                {1, 1}

        };

        // Convert from Msfl data to SDL data
        // probably not the most efficient mesh, as for each vertex we created a triangle
        // with the vertex, the center and the next vertex
        SDL_Vertex sdl_vertices[p.nb_vertices()*3];

        for (int i=0; i<p.nb_vertices(); i++) {
            Vec2D current_v = world_to_screen(p.get_global_vertex(i));
            Vec2D next_v = world_to_screen(p.get_global_vertex((i+1) % p.nb_vertices()));

            sdl_vertices[i*3] = {
                    static_cast<float>(current_v.x),
                    static_cast<float>(current_v.y),
                    (SDL_Color) color,
                    {1, 1}
            };

            sdl_vertices[i*3 + 1] = center_vertex;

            sdl_vertices[i*3 + 2] = {
                    static_cast<float>(next_v.x),
                    static_cast<float>(next_v.y),
                    (SDL_Color) color,
                    {1, 1}
            };
        }

        // Draw the polygon
        int r = SDL_RenderGeometry(renderer, nullptr, sdl_vertices, p.nb_vertices()*3, nullptr, 0);
        if (r != 0) {sdl_failure();}
    }



    void Interface::draw_text(const char *text, const Vec2D &pos, const Color4 &color) const {
        // Destination rect
        Vec2D screen_pos = world_to_screen(pos);
        SDL_Rect dest;
        dest.x = screen_pos.x;
        dest.y = screen_pos.y;
        TTF_SizeText(font, text, &dest.w, &dest.h);

        SDL_Surface * text_surface = TTF_RenderText_Blended(font, text, (SDL_Color) color);
        if (text_surface == nullptr) {sdl_ttf_failure();}
        SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_RenderCopy(renderer, text_texture, nullptr, &dest);
    }




    std::tuple<Vec2D, Vec2D> Interface::get_screen_bounds() const {
        return {screen_to_world({0, 0}), screen_to_world(window_size - Vec2D(1,1))};
    }


    void Interface::create_camera_window() {
        ImGui::Begin("Camera control", nullptr, IMGUI_WINDOW_FLAGS);

        int cam_pos[2] = {static_cast<int>(camera_pos.x), static_cast<int>(camera_pos.y)};
        ImGui::Text("Camera position");
        ImGui::SameLine();
        ImGui::PushItemWidth(75);
        ImGui::InputInt2("##", cam_pos, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Reset##camera")) {camera_pos = {0, 0};}


        ImGui::Text("Zoom");
        ImGui::SameLine();
        ImGui::PushItemWidth(152);
        ImGui::InputDouble("##", &camera_zoom_lvl, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Reset##zoom")) {camera_zoom_lvl = 20;}


        ImGui::End();
    }

    void Interface::create_debug_tools_window() {
        ImGui::Begin("Debug tools", nullptr, IMGUI_WINDOW_FLAGS);
        ImGui::Checkbox("Show shape & body centers", &debug_centers);
        ImGui::Checkbox("Show body IDs", &debug_bodyids);

        ImGui::End();
    }





    void Interface::update() {
        update_grabbing();


    }



    void Interface::update_grabbing() {
        if (selected_body != nullptr) {
            selected_body->move(screen_to_world(get_mouse_pos() + selection_pixel_offset));
        }
    }

    bool Interface::is_body_hovered(const std::shared_ptr<Body>& body) const {
        bool hovered = false;
        Vec2D mouse_pos = screen_to_world(get_mouse_pos());
        for (auto& s: body->get_shapes()) {
            if (s->is_point_inside(mouse_pos)) {
                hovered = true;
                break;
            }
        }

        return hovered;
    }

    void Interface::create_world_info_window() {
        ImGui::Begin("World informations", nullptr, IMGUI_WINDOW_FLAGS);
        ImGui::Text("Body count: %d", world->nb_bodies());
        ImGui::End();

    }
} // Msfl2Demo
