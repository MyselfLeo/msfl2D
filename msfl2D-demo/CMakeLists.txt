set(CMAKE_MODULE_PATH "P:/lib")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# ImGui is used by the demo
add_subdirectory("extern/imgui")

message("${CMAKE_MODULE_PATH}")

add_executable(
    msfl2D-demo

    main.cpp
        Interface.cpp Interface.hpp Color4.cpp Color4.hpp)

target_link_libraries(msfl2D-demo msfl2D imgui ${SDL2_STATIC_LIBRARIES})

target_include_directories(msfl2D-demo PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}"
                          "extern/imgui")