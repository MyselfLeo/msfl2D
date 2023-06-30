//
// Created by myselfleo on 27/06/2023.
//

#ifndef MSFL2D_WORLDRENDERER_HPP
#define MSFL2D_WORLDRENDERER_HPP

#include <memory>

#include <SDL2/SDL.h>
#include "msfl2D/World.hpp"

#include "Color4.hpp"
#include "msfl2D/ConvexPolygon.hpp"

using namespace Msfl2D;

namespace Msfl2Demo {
    class WorldRenderer {
    public:
        std::shared_ptr<World> world;

        /**
         * Create a new WorldRenderer. The renderer will not be creating a window yet. For that,
         * call init_window().
         */
        explicit WorldRenderer(std::shared_ptr<World> world);

        /**
         * Destroy SDL and ImGui context by calling reset(), then destroy the object.
         */
        ~WorldRenderer();

        /**
         * Open a window for this renderer.
         */
        void init_window(const Vec2D& size, const std::string& name);

        /**
         * Reset the WorldRenderer. The world is kept but all SDL and ImGui context is cleared.
         * Call this function to clean before calling, for example, SDL_Quit().
         */
        void reset();

        /**
         * Render a new frame to the screen, cleaning the last one in the process.
         * The ImGui elements are updated here.
         */
        void render();

        /**
         * Process the given SDL event
         */
        void process(const SDL_Event* event);

        /**
         * Check for non-event IO (prolonged key press for example).
         */
        void process_io();

    private:
        // Only one world renderer must be created. The creation will fail if one still exists.
        static int NB_CREATED;

        // SDL flags
        static const int WINDOW_FLAGS = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
        static const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;

        // ImGui flags
        static const int IMGUI_WINDOW_FLAGS = ImGuiWindowFlags_AlwaysAutoResize;

        static constexpr double CAMERA_SPEED = 0.5;

        // Default color for rendering
        static const Color4 BACKGROUND_COLOR;
        static const Color4 BACKGROUND_INFO_COLOR;
        static const Color4 MAIN_COLOR;
        static const Color4 COLOR_RED;
        static const Color4 COLOR_GREEN;
        static const Color4 COLOR_BLUE;
        static const Color4 COLOR_YELLOW;

        SDL_Window * window;
        SDL_Renderer * renderer;

        std::string window_name;
        Vec2D window_size;

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
        double camera_zoom_lvl = 20;

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
        
        
        // Drawing methods
        /** Draw background info (axis, etc.) */
        void draw_background(const Color4& color = BACKGROUND_INFO_COLOR) const;
        /** Draw a line */
        void draw_line(const Line& line, const Color4& color = MAIN_COLOR) const;
        /** Draw a segment of a given line */
        void draw_segment(const Segment& segment, const Line& line, const Color4& color = COLOR_YELLOW) const;
        /** Draw a point */
        void draw_point(const Vec2D& point, int size = 3, const Color4& color = COLOR_BLUE) const;
        /** Draw a ConvexPolygon outline */
        void draw_polygon_outline(const ConvexPolygon& polygon, const Color4& color = MAIN_COLOR);
        /** Draw a filled ConvexPolygon */
        void draw_polygon_filled(const ConvexPolygon& polygon, const Color4& color = MAIN_COLOR);


        // ImGui window creation methods
        void create_camera_window();


        /**
         * Return the world-space position of the top-left and bottom-right corners of the screen.
         */
        [[nodiscard]] std::tuple<Vec2D, Vec2D> get_screen_bounds() const;
    };

} // Msfl2Demo

#endif //MSFL2D_WORLDRENDERER_HPP
