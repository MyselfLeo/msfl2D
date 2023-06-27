//
// Created by myselfleo on 27/06/2023.
//

#ifndef MSFL2D_WORLDRENDERER_HPP
#define MSFL2D_WORLDRENDERER_HPP

#include <memory>

#include <SDL2/SDL.h>
#include "msfl2D/World.hpp"

#include "Color4.hpp"

namespace Msfl2Demo {

    /**
     * Main class managing the rendering of a world to a window. It also manages I/O.
     */
    class WorldRenderer {
    public:
        std::shared_ptr<Msfl2D::World> world;

        /**
         * Create a new WorldRenderer. The renderer will not be creating a window yet. For that,
         * call init_window().
         */
        explicit WorldRenderer(std::shared_ptr<Msfl2D::World> world);

        /**
         * Destroy SDL and ImGui context by calling reset(), then destroy the object.
         */
        ~WorldRenderer();

        /**
         * Open a window for this renderer.
         */
        void init_window(const Msfl2D::Vec2D& size, const std::string& name);

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
    private:
        // Only one world renderer must be created. The creation will fail if one still exists.
        static int NB_CREATED;

        // SDL flags
        const int WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;

        // Default color for rendering
        const Color4 BACKGROUND_COLOR = {0, 0, 0};
        const Color4 BACKGROUND_INFO_COLOR = {50, 50, 50};
        const Color4 MAIN_COLOR = {255, 255, 255};
        const Color4 COLOR_RED = {255, 0, 0};
        const Color4 COLOR_GREEN = {0, 255, 0};
        const Color4 COLOR_BLUE = {0, 0, 255};
        const Color4 COLOR_YELLOW = {255, 255, 0};

        SDL_Window * window;
        SDL_Renderer * renderer;

        std::string window_name;
        Msfl2D::Vec2D window_size;

        /**
         * World-space of the center of the camera (i.e center of the screen).
         * Changing this value will translate the whole world.
         */
        Msfl2D::Vec2D camera_pos;

        /**
         * Size ratio between screen-space and world-space.
         * In example, a value of 10 will mean that a distance of 1 unit in the world-space will be
         * 10 pixels wide in the screen-space.
         */
        double camera_zoom_lvl = 20;

        /** Convert coordinates from the world-space to the screen-space */
        Msfl2D::Vec2D world_to_screen(const Msfl2D::Vec2D& coo) const;
        /** Convert coordinates from the screen-space to the world-space */
        Msfl2D::Vec2D screen_to_world(const Msfl2D::Vec2D& coo) const;

        /**
         * Call SDL_SetRenderDrawColor with the given color.
         */
        void set_color(Color4 color);
    };

} // Msfl2Demo

#endif //MSFL2D_WORLDRENDERER_HPP
