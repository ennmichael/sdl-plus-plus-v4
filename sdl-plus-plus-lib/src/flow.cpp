#include "flow.hpp"
#include "boost/optional.hpp"
#include <utility>

namespace Sdl {
namespace internal {
Event event_for_raw_type(SDL_EventType event_type, const Event& event)
{
    return [event_type, event](const auto& raw_event)
    {
        if (raw_event.type == event_type)
            return event(raw_event);
        return false;
    };
}

Event key_event(SDL_EventType event_type, SDL_Scancode scancode)
{
    return internal::event_for_raw_type(event_type, [scancode](const auto& raw_event) {
        return raw_event.key.keysym.scancode == scancode;
    });
}
}

namespace Events {
Event either(const std::vector<Event>& events)
{
    return [events](const auto& raw_event)
    {
        return std::any_of(events.cbegin(), events.cend(), [&](const auto& event) {
            return event(raw_event);
        });
    };
}

Event key_down(SDL_Scancode scancode)
{
    return internal::key_event(SDL_KEYDOWN, scancode);
}

Event key_up(SDL_Scancode scancode)
{
    return internal::key_event(SDL_KEYUP, scancode);
}

Event mouse_click_inside_area(Rectangle area)
{
    return internal::event_for_raw_type(SDL_MOUSEBUTTONDOWN, [area](const auto& raw_event) {
        const Point mousePosition = {raw_event.button.x, raw_event.button.y};
        return mousePosition.is_inside_rectangle(area);
    });
}

Event window_state_changed()
{
    return internal::event_for_raw_type(SDL_WINDOWEVENT);
}

Event quit_requested()
{
    return internal::event_for_raw_type(SDL_QUIT);
}
}

void Event_dispatcher::on_event(Event event, Action action)
{
    event_action_pairs_.push_back({std::move(event), std::move(action)});
}

void Event_dispatcher::dispatch() const
{
    const auto poll_raw_event = []() -> boost::optional<SDL_Event>
    {
        SDL_Event result;
        if (SDL_PollEvent(&result))
            return result;
        return boost::none;
    };
    
    const auto dispatch_raw_event = [&](const SDL_Event& raw_event)
    {
        for (const auto& event_action_pair : event_action_pairs_)
            if (event_action_pair.event(raw_event))
                event_action_pair.action();
    };
    
    while (const auto raw_event = poll_raw_event())
        dispatch_raw_event(*raw_event);
}

const char* Main_loop::Already_running::what() const noexcept
{
    return "Trying to start an already running loop.";
}

const char* Main_loop::Already_stopped::what() const noexcept
{
    return "Trying to stop a loop that isn't running.";
}

void Main_loop::on_event(Event event, Action action)
{
    event_dispatcher_.on_event(event, action);
}

void Main_loop::start() noexcept
{
    is_running_ = true;
    while (is_running_) {
        event_dispatcher_.dispatch();
    }
}

void Main_loop::stop() noexcept 
{
    is_running_ = false;
}

bool Main_loop::is_running() const noexcept 
{
    return is_running_;
}
}
