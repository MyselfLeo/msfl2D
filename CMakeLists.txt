cmake_minimum_required(VERSION 3.25)
project(msfl2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)


# msfl2D is the main library.
add_subdirectory(msfl2D)
# msfl2D-demo is an executable, to show you how you could use msfl2D
add_subdirectory(msfl2D-demo)