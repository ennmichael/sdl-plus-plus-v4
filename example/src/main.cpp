#include "sdl-plus-plus-v4/sdl-plus-plus.hpp"

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
    
    Main_loop main_loop;
    
    auto background_color = Colors::white();
    
    main_loop.on_event(Events::quit_requested(), [&] {
        main_loop.stop();
    });
    
    const auto redraw = [&] {
        const Line line = {
            {0, 0},
            {20, 20}
        };
        wnd.draw_line(line, Colors::red());
        wnd.refresh(background_color);
    };
    
    main_loop.on_event(Events::window_state_changed(), [&] {
        redraw();
    });
    
    main_loop.on_event(Events::key_down(SDL_SCANCODE_RETURN), [&] {
        std::cout << "Return pressed\n";
        if (background_color == Colors::white())
            background_color = Colors::black();
        else
            background_color = Colors::white();
        redraw();
    });

    main_loop.start();
}
