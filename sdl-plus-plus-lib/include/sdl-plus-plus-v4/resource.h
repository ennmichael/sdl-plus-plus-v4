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

struct Texture_deleter {
    void operator()(SDL_Texture*) const noexcept;
};

struct Surface_deleter {
    void operator()(SDL_Surface*) const noexcept;
};

struct Renderer_deleter {
    void operator()(SDL_Renderer*) const noexcept;
};

struct Window_deleter {
    void operator()(SDL_Window*) const noexcept;
};

using Unique_renderer = std::unique_ptr<SDL_Renderer, Renderer_deleter>;
using Unique_window = std::unique_ptr<SDL_Window, Window_deleter>;
using Unique_surface = std::unique_ptr<SDL_Surface, Surface_deleter>;
using Unique_texture = std::unique_ptr<SDL_Texture, Texture_deleter>;

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
