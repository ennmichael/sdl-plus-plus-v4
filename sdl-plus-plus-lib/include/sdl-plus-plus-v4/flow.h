#ifndef SDLPP__FLOW__HEADER_
#define SDLPP__FLOW__HEADER_

#include "drawing.h"
#include <chrono>
#include <stdexcept>
#include <utility>

namespace Sdl {
enum class Event_result : bool { quit, proceed };

bool key_down(SDL_Scancode);

class Mouse_motion_tracker {
   public:
    void update(const SDL_Event&) noexcept;
    Sdl::Point mouse_position() const noexcept;

   private:
    Sdl::Point pos_{};
};

template <class Default_handler, class Event_handler>
class Event_loop {
   public:
    template <class Default_handler_t, class Event_handler_t>
    Event_loop(
        Default_handler_t&& default_handler,
        Event_handler_t&& event_handler)
        : default_handler_(std::forward<Default_handler_t>(default_handler)),
          event_handler_(std::forward<Event_handler_t>(event_handler)) {}

    void start() {
        while (!quit_) {
            poll_and_handle_events();
            handle_default();
        }
    }

   private:
    void handle_default() { default_handler_(); }

    void poll_and_handle_events() {
        for (SDL_Event event; SDL_PollEvent(&event);) {
            handle_event(event);
        }
    }

    void handle_event(const SDL_Event& event) {
        Event_result event_result = event_handler_(event);
        check_quit(event_result);
    }

    void check_quit(Event_result event_result) noexcept {
        if (quit_requested(event_result))
            set_quit_flag();
    }

    bool quit_requested(Event_result event_result) const noexcept {
        return event_result == Event_result::quit;
    }

    void set_quit_flag() noexcept { quit_ = true; }

    Default_handler default_handler_;
    Event_handler event_handler_;
    bool quit_ = false;
};

template <class Default_handler, class Event_handler>
auto make_event_loop(
    Default_handler&& default_handler,
    Event_handler&& event_handler) {
    return Event_loop<Default_handler, Event_handler>(
        std::forward<Default_handler>(default_handler),
        std::forward<Event_handler>(event_handler));
}

class Timer {
   public:
    struct Invalid_delay : std::exception {
        const char* what() const noexcept override;
    };

    explicit Timer(std::chrono::milliseconds);

    bool ready() noexcept;
    std::chrono::milliseconds get_delay() const noexcept;

   private:
    bool delay_too_large() const noexcept;

    std::chrono::milliseconds delay_;
    Uint32 current_ = SDL_GetTicks();
};
}

#endif
