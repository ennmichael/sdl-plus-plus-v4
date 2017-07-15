cmake_minimum_required(VERSION 3.0.2)
project(sdl_plus_plus_lib)

add_library(sdl_plus_plus_lib src/drawing.cpp src/flow.cpp src/resource.cpp)

macro(add_own_includes)
    target_include_directories(sdl_plus_plus_lib PRIVATE include/sdl-plus-plus-v4)
    target_include_directories(sdl_plus_plus_lib PUBLIC include)
endmacro()

macro(extend_module_path)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${sdl_plus_plus_lib_SOURCE_DIR}/cmake")
endmacro()

macro(find_sdl)
    find_package(SDL2fallback REQUIRED)
    find_package(SDL2_image REQUIRED)
endmacro()

macro(add_sdl)
    target_include_directories(sdl_plus_plus_lib PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(sdl_plus_plus_lib ${SDL2_LIBRARY})
    target_include_directories(sdl_plus_plus_lib PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(sdl_plus_plus_lib ${SDL2_IMAGE_LIBRARIES})
endmacro()

macro(find_boost)
    find_package(Boost 1.61 REQUIRED)
endmacro()

macro(add_boost)
    target_include_directories(sdl_plus_plus_lib PRIVATE ${Boost_INCLUDE_DIRS})
endmacro()

macro(add_flags)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (app PRIVATE -Wall -Wextra -pedantic -Werror -std=c++14)
    endif()
endmacro()

add_own_includes()
extend_module_path()

find_sdl()
add_sdl()

find_boost()
add_boost()
