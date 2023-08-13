# msfl2D - A 2D physics engine

**msfl2D** is my essay at creating a working 2D physics engine in C++. This is a toy project, still in development,
that I started with the goal of understanding how a physics engine works.

## Repository

The repository is currently separated in 2 CMake targets, `msfl2D` and `msfl2D-demo`. `msfl2D` is the actual
C++ library, while (as its name suggests) `msfl2D-demo` is an implementation of **msfl2D** using [SDL2](https://www.libsdl.org/)
and [Dear ImGui](https://github.com/ocornut/imgui).

## State of the project

Keep in mind that this project is my first attempt to build such software.
Right now, the linear collisions work, and I am implementing angular collisions, not without pain.

## License

This project is licensed under **Mozilla Public License 2.0**. See `LICENSE.txt`.