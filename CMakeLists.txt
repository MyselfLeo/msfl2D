cmake_minimum_required(VERSION 3.25)
project(msfl2D)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(msfl2D)

add_executable(msfl2D-Demo demo.cpp)
target_link_libraries(msfl2D-Demo msfl2D)

target_include_directories(msfl2D-Demo PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/msfl2D")