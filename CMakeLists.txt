cmake_minimum_required(VERSION 3.10)

project(raylib)

set(CC clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

add_executable(main main.c)

target_link_libraries(main PRIVATE
    raylib
)
