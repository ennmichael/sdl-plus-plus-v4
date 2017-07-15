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
    
    Main_loop main_loop;
    
    main_loop.on_event(Events::quit_requested(), [&] {
        main_loop.stop();
    });
    
    const Line line = {
        {0, 0},
        {20, 20}
    };
    wnd.draw_line(line, Colors::red());
    wnd.redraw(Colors::white());

    main_loop.start();
}
