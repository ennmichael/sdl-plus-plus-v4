#pragma once

#include "drawing.h"
#include <chrono> // TODO add the timer, or a timing event?
#include <stdexcept>
#include <functional>
#include <vector>

namespace Sdl {
using Action = std::function<void()>;
using Event = std::function<bool(const SDL_Event&)>;

namespace Events {
namespace internal {
Event key_event(SDL_EventType, SDL_Scancode);
}

Event key_down(SDL_Scancode);
Event key_up(SDL_Scancode);
Event mouse_click_inside_area(Rectangle);
Event quit_requested();

// A special eny event???

// More various events
};

class Event_dispatcher {
public:
    void on_event(Event, Action);
    void dispatch();
    
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
