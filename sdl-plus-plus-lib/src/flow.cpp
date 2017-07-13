#include "flow.h"
#include "SDL2/SDL.h"

namespace Sdl {
bool key_down(SDL_Scancode scancode) {
    return SDL_GetKeyboardState(nullptr)[scancode] == 1;
}

void Mouse_motion_tracker::update(const SDL_Event& e) noexcept {
    if (e.type == SDL_MOUSEMOTION)
        pos_ = {e.motion.x, e.motion.y};
}

Sdl::Point Mouse_motion_tracker::mouse_position() const noexcept {
    return pos_;
}

const char* Timer::Invalid_delay::what() const noexcept {
    return "The delay length is too large; must be less than "
           "std::numeric_limits<Uint32>::max().";
}

Timer::Timer(std::chrono::milliseconds delay) : delay_(delay) {
    if (delay_too_large())
        throw Invalid_delay();
}

bool Timer::ready() noexcept {
    auto ticks = SDL_GetTicks();
    if (ticks >= delay_.count() + current_) {
        current_ = ticks;
        return true;
    }
    return false;
}

bool Timer::delay_too_large() const noexcept {
    return delay_.count() >= std::numeric_limits<Uint32>::max();
}

std::chrono::milliseconds Timer::get_delay() const noexcept {
    return std::chrono::milliseconds{delay_};
}
}
