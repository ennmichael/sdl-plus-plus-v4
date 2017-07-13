#include "sdl-plus-plus-v4/flow.h"
#include "sdl-plus-plus-v4/resource.h"
#include "sdl-plus-plus-v4/drawing.h"

#include <iostream>

using namespace Sdl;

int main(int, char**) {
    Sdl_raii _;

    {
        std::unordered_map<Point, std::string> map;
        map[Point(0, 0)] = "Zero, Zero";
        map[Point(10, 10)] = "Ten, Ten";

        for (const auto& elem : map)
            std::cout << elem.first << " = " << elem.second << '\n';

        Point p1(10, 10);
        Point p2(2, 5);
        std::cout << "+ " << (p1 + p2) << '\n';
        std::cout << "- " << (p1 - p2) << '\n';
        std::cout << "* " << (p1 * p2) << '\n';
        std::cout << "/ " << (p1 / p2) << '\n';
    }

    auto props = Window_properties::default_values_with_title("Title");
    Window wnd(props);
    Mouse_motion_tracker mouse_motion_tracker;

    auto event_loop = make_event_loop(
        [&] {
            wnd.draw_line(
                Line(
                    Point(props.size.width / 2, props.size.height / 2),
                    mouse_motion_tracker.mouse_position()),
                Colors::black());

            Rectangle_points points;
            points.lower_right = mouse_motion_tracker.mouse_position();
            points.upper_left =
                Point(props.size.width / 2, props.size.height / 2);

            wnd.draw_rectangle(
                make_rectangle(points), Colors::black(), Color_filling::none);
            wnd.redraw(Colors::white());
        },
        [&](const SDL_Event& e) noexcept {
            if (e.type == SDL_QUIT)
                return Event_result::quit;
            mouse_motion_tracker.update(e);
            return Event_result::proceed;
        });

    event_loop.start();
}
