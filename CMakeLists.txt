cmake_minimum_required(VERSION 3.25)
project(msfl2D)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "P:/lib")

# msfl2D (demo.cpp is a demo renderer)
add_subdirectory(msfl2D)

# External libraries
add_subdirectory(extern/imgui)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(msfl2D-Demo demo.cpp)
target_link_libraries(msfl2D-Demo msfl2D imgui ${SDL2_STATIC_LIBRARIES})

target_include_directories(msfl2D-Demo PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/msfl2D"
                          "${PROJECT_SOURCE_DIR}/extern/imgui")