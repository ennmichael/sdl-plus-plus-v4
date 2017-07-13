#pragma once

#include "SDL2/SDL.h"
#include "resource.h"
#include "boost/operators.hpp"
#include "boost/functional/hash.hpp"
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>
#include <cmath>

namespace Sdl {
using Color = SDL_Color;
using Rectangle = SDL_Rect;

struct Point;
using Point_vector = std::vector<Point>;

template <class CRTP>
struct Has_basic_operations : boost::equality_comparable<CRTP, 
                              boost::arithmetic<CRTP>>
{};

struct Point : private Has_basic_operations<Point> {
    static Point origin() noexcept;

    Point() = default;
    Point(int, int) noexcept;

    int x;
    int y;

    bool operator==(Point) const noexcept;
    Point& operator+=(Point) noexcept;
    Point& operator-=(Point) noexcept;
    Point& operator*=(Point) noexcept;
    Point& operator/=(Point) noexcept;
};

std::ostream& operator<<(std::ostream&, Point);

struct Size : private Has_basic_operations<Size> {
    Size() = default;
    Size(int, int) noexcept;

    int width;
    int height;

    bool operator==(Size) const noexcept;
    Size& operator+=(Size) noexcept;
    Size& operator-=(Size) noexcept;
    Size& operator*=(Size) noexcept;
    Size& operator/=(Size) noexcept;
};

std::ostream& operator<<(std::ostream&, Size);

double distance_between_points(Point, Point) noexcept;

struct Message_box {
    enum class Type : Uint32 {
        Basic = 0,
        Error = SDL_MESSAGEBOX_ERROR,
        Warning = SDL_MESSAGEBOX_WARNING,
        Info = SDL_MESSAGEBOX_INFORMATION
    };

    std::string title;
    std::string content;
    Type type = Type::Basic;
};

void show_message_box(const Message_box&);

namespace Colors {
constexpr Color red(Uint8 amount = 255, Uint8 alpha = 255) noexcept {
    return {amount, 0, 0, alpha};
}

constexpr Color green(Uint8 amount = 255, Uint8 alpha = 255) noexcept {
    return {0, amount, 0, alpha};
}

constexpr Color blue(Uint8 amount = 255, Uint8 alpha = 255) noexcept {
    return {0, 0, amount, alpha};
}

constexpr Color yellow(Uint8 amount = 255, Uint8 alpha = 255) noexcept {
    return {amount, amount, 0, alpha};
}

constexpr Color black(Uint8 alpha = 255) noexcept {
    return {0, 0, 0, alpha};
}

constexpr Color white(Uint8 alpha = 255) noexcept {
    return {255, 255, 255, alpha};
}
}

struct Rectangle_points {
    Point upper_left;
    Point lower_right;
};

Size rectangle_size(Rectangle_points) noexcept;
Rectangle make_rectangle(Rectangle_points) noexcept;
Rectangle make_rectangle(Point, Size) noexcept;

class Angle {
public:
    static Angle pi() noexcept;
    
    static Angle in_degrees(double) noexcept;
    static Angle in_rads(double) noexcept;
    
    double degrees() const noexcept;
    double rads() const noexcept;
    
private:
    Angle(double rads, double degrees);
    double rads_;
    double degrees_;
};

struct Line {
    using Length = int;
    
    Line() = default;
    Line(Point, Angle, Length) noexcept;
    Line(Point, Point) noexcept;

    Point from{};
    Point to{};
};

std::ostream& operator<<(std::ostream&, Line);

using Filename = std::string;

class Texture {
public:
    static Texture from_raw_surface(SDL_Renderer&, SDL_Surface&);

    Size size() const;
    void draw_to_raw_renderer(SDL_Renderer&, Point);

private:
    explicit Texture(SDL_Renderer&, SDL_Surface&);

    Rectangle create_source_rectangle() const noexcept;
    Rectangle create_destination_rectangle(Point) const noexcept;
    
    Unique_texture texture_ptr_;
};

class Surface {
public:
    static Surface load_from_file(const Filename&);

    Texture get_texture_from_raw_renderer(SDL_Renderer&);

private:
    explicit Surface(const Filename&);

    Unique_surface surface_ptr_;
};

enum class Color_filling : bool { filled, none };

class Renderer {
public:
    explicit Renderer(SDL_Window&);

    Texture load_texture(const Filename&);
    void set_blend_mode(SDL_BlendMode);
    void render_present();
    void render_clear(Color);

    void draw_texture(Texture&, Point);
    void draw_line(Line, Color);
    void draw_rectangle(Rectangle, Color, Color_filling);

private:
    void set_draw_color(Color);
    void draw_filled_rectangle(Rectangle);
    void draw_empty_rectangle(Rectangle);
    Color get_draw_color() const;
    
    Unique_renderer renderer_ptr_;
};

class Image_cache {
public:
    struct Getting_uncached_image : std::exception {
        const char* what() const noexcept override;
    };
    
    void cache_image(const Filename&, Renderer&);
    bool image_is_cached(const Filename&) const noexcept;
    Texture& image_texture(const Filename&);
    const Texture& image_texture(const Filename&) const;

private:
    template <class Self>
    static auto& get_image_texture_impl(Self& self, const Filename& filename) {
        try {
            return self.textures_.at(filename);
        } catch(const std::out_of_range&) {
            throw Getting_uncached_image{};
        };
    };
    
    std::unordered_map<Filename, Texture> textures_;
};

struct Window_properties {
    static Window_properties default_values();
    static Window_properties default_values_with_title(std::string);

    Size size;
    Point position;
    std::string title;
};

class Window {
public:
    explicit Window(const Window_properties&, Uint32 = 0);

    void draw_image(const Filename&, Point);
    void draw_line(Line, Color);
    void draw_rectangle(Rectangle, Color, Color_filling);

    void redraw(Color); // How about no? Clear and draw should be split 
                        // and redraw should be a convenience function

private:
    class Draw_commands {
    public:
        using Command = std::function<void()>;
        
        void add_command(const Command&);
        void execute();
        
    private:
        std::vector<Command> draw_commands_;
    };
    
    using Draw_command = std::function<void()>;
    
    void cache_image(const Filename&);
    void execute_draw_commands();
    static Unique_window create_checked_window(const Window_properties&, Uint32);
    static Unique_window create_unchecked_window(const Window_properties&, Uint32);

    Image_cache image_cache_;
    Unique_window window_ptr_;
    Renderer renderer_;
    Draw_commands draw_commands_;
};
}

namespace std {
template <>
class hash<Sdl::Point> {
   public:
    std::size_t operator()(Sdl::Point point) const noexcept {
        std::size_t seed = 0;
        boost::hash_combine(seed, point.x);
        boost::hash_combine(seed, point.y);
        return seed;
    }
};
}

namespace std {
template <>
class hash<Sdl::Size> {
   public:
    std::size_t operator()(Sdl::Size size) const noexcept {
        std::size_t seed = 0;
        boost::hash_combine(seed, size.width);
        boost::hash_combine(seed, size.height);
        return seed;
    }
};
}
