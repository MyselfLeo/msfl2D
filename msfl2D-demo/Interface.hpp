//
// Created by myselfleo on 27/06/2023.
//

#ifndef MSFL2D_INTERFACE_HPP
#define MSFL2D_INTERFACE_HPP

#include <memory>

#include <SDL2/SDL.h>
#include "msfl2D/World.hpp"

#include "Color4.hpp"
#include "msfl2D/ConvexPolygon.hpp"

using namespace Msfl2D;

namespace Msfl2Demo {
    class Interface {
    public:
        std::shared_ptr<World> world;

        // Default color for rendering
        static const Color4 BACKGROUND_COLOR;
        static const Color4 BACKGROUND_INFO_COLOR;
        static const Color4 SHAPE_OUTLINE_COLOR;
        static const Color4 SHAPE_AREA_COLOR;
        static const Color4 COLOR_RED;
        static const Color4 COLOR_GREEN;
        static const Color4 COLOR_BLUE;
        static const Color4 COLOR_YELLOW;

        /**
         * Create a new Interface. The renderer will not be creating a window yet. For that,
         * call init_window().
         */
        explicit Interface(std::shared_ptr<World> world);

        /**
         * Destroy SDL and ImGui context by calling reset(), then destroy the object.
         */
        ~Interface();

        /**
         * Open a window for this renderer.
         */
        void init_window(const Vec2D& size, const std::string& name);

        /**
         * Reset the Interface. The world is kept but all SDL and ImGui context is cleared.
         * Call this function to clean before calling, for example, SDL_Quit().
         */
        void reset();

        /**
         * Render a new frame to the screen, cleaning the last one in the process.
         * The ImGui elements are updated here.
         * @param update Whether or not to update the screen at the end of this function. Set it to false
         *   if you want to draw something to the screen after calling this function. Be sure to call "update_screen"
         *   afterward!
         */
        void render(bool update = true);

        /**
         * Update manually the screen. To be used only after a call to render() with update = false.
         */
        void update_screen();

        /**
         * Process the given SDL event
         */
        void process(const SDL_Event* event);

        /**
         * Check for non-event IO (prolonged key press for example).
         */
        void process_io();

        /**
         * Update the world
         */
        void update();




        // Drawing methods
        void draw_body(const std::shared_ptr<Body>& body) const;
        /** Draw background info (axis, etc.) */
        void draw_background(const Color4& color = BACKGROUND_INFO_COLOR) const;
        /** Draw a line */
        void draw_line(const Line& line, const Color4& color = SHAPE_OUTLINE_COLOR) const;
        /** Draw a LineSegment */
        void draw_segment(const LineSegment& segment, const Color4& color = COLOR_YELLOW) const;
        /** Draw a point */
        void draw_point(const Vec2D& point, int size = 3, const Color4& color = COLOR_BLUE) const;
        /** Draw a ConvexPolygon outline */
        void draw_polygon_outline(const ConvexPolygon& polygon, const Color4& color = SHAPE_OUTLINE_COLOR) const;
        /** Draw a filled ConvexPolygon */
        void draw_polygon_filled(const ConvexPolygon& polygon, const Color4& color = SHAPE_AREA_COLOR) const;


    private:
        // Only one world renderer must be created. The creation will fail if one still exists.
        static int NB_CREATED;

        // SDL flags
        static const int WINDOW_FLAGS = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
        static const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;

        // ImGui flags
        static const int IMGUI_WINDOW_FLAGS = ImGuiWindowFlags_AlwaysAutoResize;

        static constexpr double CAMERA_SPEED = 0.75;

        SDL_Window * window;
        SDL_Renderer * renderer;

        std::string window_name;
        Vec2D window_size;

        // Used to manage grabbing and moving bodies.
        std::shared_ptr<Body> selected_body;
        Vec2D selection_pixel_offset = {0, 0};

        /**
         * World-space of the center of the camera (i.e center of the screen).
         * Changing this value will translate the whole world.
         */
        Vec2D camera_pos;

        /**
         * Size ratio between screen-space and world-space.
         * In example, a value of 10 will mean that a distance of 1 unit in the world-space will be
         * 10 pixels wide in the screen-space.
         */
        double camera_zoom_lvl = 50;


        // Flags to display specific debug infos
        bool debug_centers = false;

        /** Convert coordinates from the world-space to the screen-space */
        Vec2D world_to_screen(const Vec2D& coo) const;
        /** Convert coordinates from the screen-space to the world-space */
        Vec2D screen_to_world(const Vec2D& coo) const;

        /**
         * Call SDL_SetRenderDrawColor with the given color.
         */
        void set_color(Color4 color) const;
        
        
        /**
         * Return the position of the mouse on the screen.
         */
        static Vec2D get_mouse_pos() ;

        // ImGui window creation methods
        void create_camera_window();
        void create_debug_tools_window();
        void create_world_info_window();


        // Methods used to update the World.
        /**
         * Manage grabbing & dropping bodies.
         */
        void update_grabbing();



        [[nodiscard]] bool is_body_hovered(const std::shared_ptr<Body>& body) const;


        /**
         * Return the world-space position of the top-left and bottom-right corners of the screen.
         */
        [[nodiscard]] std::tuple<Vec2D, Vec2D> get_screen_bounds() const;
    };

} // Msfl2Demo

#endif //MSFL2D_INTERFACE_HPP
