cmake_minimum_required(VERSION 3.0.2)
project(example)

add_executable(example src/main.cpp)
target_link_libraries(example sdl_plus_plus_lib)
