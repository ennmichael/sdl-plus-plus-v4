#include "resource.h"

namespace Sdl {
const char* Sdl_exception::what() const noexcept 
{
    return SDL_GetError();
}

Sdl_raii::Sdl_raii(Uint32 flags) 
{
    internal::error_checking::check_function(SDL_Init(flags));
}

Sdl_raii::~Sdl_raii() 
{
    SDL_Quit();
}

Img_raii::Img_raii(int flags) {
    IMG_Init(flags);
}

Img_raii::~Img_raii()
{
    IMG_Quit();
}

namespace internal {
namespace error_checking {
void check_function(int function_result) {
    if (function_result != 0)
        throw Sdl_exception{};
}
}
}
}
