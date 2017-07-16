#pragma once

#include "drawing.hpp"
#include "internal/utils.hpp"
#include <chrono> // TODO add a timer, or an Events::every(std::chrono::milliseconds)?
#include <stdexcept>
#include <functional>
#include <vector>

namespace Sdl {
using Action = std::function<void()>;
using Event = std::function<bool(const SDL_Event&)>;

namespace internal {
Event event_for_raw_type(SDL_EventType, const Event& = internal::utils::id(true));
Event key_event(SDL_EventType, SDL_Scancode);
}

namespace Events {
Event either(const std::vector<Event>&);

Event key_down(SDL_Scancode);
Event key_up(SDL_Scancode);
Event mouse_click_inside_area(Rectangle);
Event window_state_changed();
Event anything();
Event quit_requested();

// More various events to add
};

class Event_dispatcher {
public:
    void on_event(Event, Action);
    void dispatch() const;
    
private:
    struct Event_action_pair {
        Event event;
        Action action;
    };
    
    std::vector<Event_action_pair> event_action_pairs_;
};

class Main_loop {
public:
    struct Already_running : std::exception {
        const char* what() const noexcept override;
    };
    
    struct Already_stopped : std::exception {
        const char* what() const noexcept override;
    };
    
    void on_event(Event, Action);
    
    void start() noexcept;
    void stop() noexcept;
    bool is_running() const noexcept;
    
private:
    bool is_running_ = false;
    Event_dispatcher event_dispatcher_;
};
}
