//
// Created by myselfleo on 27/06/2023.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "imgui.h"
#include "msfl2D/World.hpp"
#include "Interface.hpp"
#include "msfl2D/CollisionDetector.hpp"
#include "msfl2D/CollisionResolver.hpp"

using namespace Msfl2D;
using namespace Msfl2Demo;



/** Initialise SDL */
void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) Interface::sdl_failure();
    if (TTF_Init() != 0) {Interface::sdl_ttf_failure();}
}







void debug_print_penetrations(Interface& interface, std::shared_ptr<ConvexPolygon>& shape1, const std::shared_ptr<ConvexPolygon>& shape2, Vec2D min_dist_point) {
    for (int i=0; i<shape1->nb_vertices(); i++) {
        // Get the side we're checking
        LineSegment tested_side = LineSegment(
                shape1->get_global_vertex(i),
                shape1->get_global_vertex((i + 1) % shape1->nb_vertices())
        );

        Vec2D side_vec = tested_side.get_vec();
        Vec2D proj_axis = {side_vec.y, -side_vec.x}; // normal of the side

        // Project the shapes onto a line with this orientation (passing through the origin because i said so)
        Line proj_line = Line::from_director_vector({0, 0}, proj_axis);
        LineSegment proj_shape_1 = shape1->project(proj_line);
        LineSegment proj_shape_2 = shape2->project(proj_line);

        LineSegment penetration = LineSegment::intersection(proj_shape_1, proj_shape_2);

        interface.draw_text(std::to_string(penetration.length()).c_str(), tested_side.center());

        double distance = min_dist_point.distance_squared(tested_side.line);
        interface.draw_text(std::to_string(distance).c_str(), tested_side.center() - Vec2D(0, 0.4), Interface::COLOR_RED);
    }
}









int main(int argc, char *argv[]) {
    init_sdl();

    // Floor
    std::shared_ptr<ConvexPolygon> floor = std::make_shared<ConvexPolygon>(ConvexPolygon({{-20, 0}, {20, 0}, {20, -1}, {-20, -1}}));
    std::shared_ptr<Body> body_floor = std::make_shared<Body>(Body());
    body_floor->add_shape(floor);
    body_floor->is_static = true;
    //body_floor->set_bounciness(0.1);


    // triangle
    /*std::shared_ptr<ConvexPolygon> shape_1 = std::make_shared<ConvexPolygon>(ConvexPolygon(3, 3, {-20, 1.5}));
    std::shared_ptr<Body> body_1 = std::make_shared<Body>(Body());
    body_1->add_shape(shape_1);
    body_1->is_static = true;
    body_1->rotate(M_PI / 2);
    body_floor->set_bounciness(0.1);*/

    // square
    std::shared_ptr<ConvexPolygon> shape_2 = std::make_shared<ConvexPolygon>(ConvexPolygon(4, 1, {-4, 100}));
    std::shared_ptr<Body> body_2 = std::make_shared<Body>(Body());
    body_2->add_shape(shape_2);
    body_2->rotate(M_PI / 4);
    body_2->set_mass(50);
    //body_2->set_bounciness(1);

    // square
    std::shared_ptr<ConvexPolygon> shape_3 = std::make_shared<ConvexPolygon>(ConvexPolygon(4, 3, {-4, 3}));
    std::shared_ptr<Body> body_3 = std::make_shared<Body>(Body());
    body_3->add_shape(shape_3);
    body_3->rotate(M_PI / 4);
    body_3->set_mass(50);
    //body_3->set_bounciness(1);

    // square
    std::shared_ptr<ConvexPolygon> shape_4 = std::make_shared<ConvexPolygon>(ConvexPolygon(4, 5, {4, 100}));
    std::shared_ptr<Body> body_4 = std::make_shared<Body>(Body());
    body_4->add_shape(shape_4);
    body_4->rotate(M_PI / 4);
    body_4->set_mass(50);
    //body_4->set_bounciness(1);



    std::shared_ptr<World> world = std::make_shared<World>(World());
    world->add_body(body_floor);
    //world->add_body(body_1);
    world->add_body(body_2);
    world->add_body(body_3);
    world->add_body(body_4);

    world->set_friction(0.1);

    Interface interface = Interface(world);

    interface.init_window({1280, 720}, "Msfl2D Demo");

    // Main loop
    bool stop = false;
    Uint64 old_time = SDL_GetTicks64();
    while (!stop) {
        Uint64 new_time = SDL_GetTicks64();
        double delta_t = (double)(new_time - old_time) / 1000;        // time since last loop in seconds
        old_time = new_time;

        // Process I/O events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            interface.process(&event);
            if (event.type == SDL_QUIT) {stop = true;} // end condition
        }


        interface.process_io();

        // if delta_t is too high, a lag may have happened (window moved for example)
        // we don't update to prevent unexpected simulation behavior
        if (delta_t < 0.5) { interface.update(delta_t); }
        interface.render(delta_t);
    }

    // Most of the cleanup is done by the Interface via reset()
    interface.reset();
    SDL_Quit();
    return EXIT_SUCCESS;
}