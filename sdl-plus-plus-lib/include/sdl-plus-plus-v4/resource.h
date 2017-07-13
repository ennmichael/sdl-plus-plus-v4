#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <memory>
#include <stdexcept>

namespace Sdl {
class Sdl_exception : public std::exception {
public:
    const char* what() const noexcept override;
};

class Sdl_raii {
public:
    Sdl_raii(Uint32 = SDL_INIT_EVERYTHING);
    ~Sdl_raii();

    Sdl_raii(const Sdl_raii&) = delete;
    Sdl_raii(Sdl_raii&&) = delete;
    Sdl_raii& operator=(const Sdl_raii&) = delete;
    Sdl_raii& operator=(Sdl_raii&&) = delete;
};

class Img_raii {
    Img_raii(int = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
    ~Img_raii();

    Img_raii(const Img_raii&) = delete;
    Img_raii(Img_raii&&) = delete;
    Img_raii& operator=(const Img_raii&) = delete;
    Img_raii& operator=(Img_raii&&) = delete;
};

namespace internal {
namespace error_checking {
template <class PossiblySmartPointer>
void check_pointer(const PossiblySmartPointer& ptr) {
    if (ptr == nullptr)
        throw Sdl_exception{};
}

void check_function(int);
}
}
}
