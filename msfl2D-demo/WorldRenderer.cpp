//
// Created by myselfleo on 27/06/2023.
//

#include "WorldRenderer.hpp"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <utility>

namespace Msfl2Demo {
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


        ImGui::Begin("hello", nullptr, IMGUI_WINDOW_FLAGS);
        ImGui::Text("hello");
        ImGui::End();


        // frame rendering
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }


    void WorldRenderer::set_color(Color4 color) {
        if (renderer == nullptr) {
            std::cerr << "Called set_color on an uninitialised WorldRenderer" << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
} // Msfl2Demo
