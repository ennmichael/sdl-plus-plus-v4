#include "drawing.h"
#include "SDL2/SDL_image.h"
#include <cmath>
#include <iostream>

namespace Sdl {
Point Point::origin() noexcept 
{
    return Point(0, 0);
}

Point::Point(int x, int y) noexcept : x{x}, y{y} 
{}

bool Point::operator==(Point rhs) const noexcept 
{
    return x == rhs.x && y == rhs.y;
}

Point& Point::operator+=(Point rhs) noexcept 
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point& Point::operator-=(Point rhs) noexcept 
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Point& Point::operator*=(Point rhs) noexcept 
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

Point& Point::operator/=(Point rhs) noexcept 
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

bool Point::is_inside_rectangle(Rectangle rect) const
{   
    const auto rect_points = as_rectangle_points(rect);
    return x > rect_points.upper_left.x && y > rect_points.lower_right.y &&
           x < rect_points.lower_right.x && y <rect_points.lower_right.y;
}

std::ostream& operator<<(std::ostream& os, Point p) 
{
    return os << "{x: " << p.x << ", y: " << p.y << '}';
}

Size::Size(int width, int height) noexcept : width{width}, height{height} 
{}

bool Size::operator==(Size rhs) const noexcept 
{
    return width == rhs.width && height == rhs.height;
}

Size& Size::operator+=(Size rhs) noexcept 
{
    width += rhs.width;
    height += rhs.height;
    return *this;
}

Size& Size::operator-=(Size rhs) noexcept 
{
    width -= rhs.width;
    height -= rhs.height;
    return *this;
}

Size& Size::operator*=(Size rhs) noexcept 
{
    width *= rhs.width;
    height *= rhs.height;
    return *this;
}

Size& Size::operator/=(Size rhs) noexcept 
{
    width /= rhs.width;
    height /= rhs.height;
    return *this;
}

std::ostream& operator<<(std::ostream& os, Size size) 
{
    os << "{w: " << size.width << ", h: " << size.height << '}';
    return os;
}

double distance_between_points(Point p1, Point p2) noexcept 
{
    auto difference = [](auto a, auto b) 
    {
        return std::abs(a - b);
    };
    
    auto pythagorean_theoreme = [](auto a, auto b) 
    {
        return std::sqrt(a*a + b*b);
    }; // Expose these please we should maybe have a header for math related stuff?
    
    return pythagorean_theoreme(difference(p1.x, p2.x), difference(p1.y, p2.y));
}

void show_message(const Message_box& mbox) 
{
    internal::error_checking::check_function(
        SDL_ShowSimpleMessageBox(
            static_cast<Uint32>(mbox.type), mbox.title.c_str(),
            mbox.content.c_str(), nullptr));
}

Size rectangle_size(Rectangle_points points) noexcept 
{
    return Size(
        points.lower_right.x - points.upper_left.x,
        points.lower_right.y - points.upper_left.y);
}

Rectangle as_rectangle(Rectangle_points rectangle_points) noexcept 
{
    return as_rectangle(
        rectangle_points.upper_left, rectangle_size(rectangle_points));
}

Rectangle as_rectangle(Point upper_left, Size size) noexcept 
{
    return {upper_left.x, upper_left.y, size.width, size.height};
}

Rectangle_points as_rectangle_points(Rectangle rect) noexcept
{
    return {
        {rect.x, rect.y},
        {rect.x+rect.w, rect.y+rect.h}
    };
}

Angle Angle::pi() noexcept 
{
    return Angle::in_rads(M_PI);
}

Angle Angle::in_degrees(double degrees) noexcept 
{
    return Angle(degrees*M_PI / 180, degrees);
}

Angle Angle::in_rads(double rads) noexcept 
{
    return Angle(rads, rads*180 / M_PI);
}

double Angle::degrees() const noexcept 
{
    return degrees_;
}

double Angle::rads() const noexcept 
{
    return rads_;
}

Angle::Angle(double rads, double degrees)
    : rads_{rads}
    , degrees_{degrees}
{}

Line::Line(Point new_from, Angle angle, int length) noexcept
    : from{new_from},
      to{static_cast<int>(std::sin(angle.rads()) * length + from.x),
         static_cast<int>(std::cos(angle.rads()) * length + from.y)}
{}
         
Line::Line(Point from, Point to) noexcept : from{from}, to{to} 
{}

std::ostream& operator<<(std::ostream& os, Line l) 
{
    return os << '{' << l.from << ", " << '}';
}

Texture Texture::from_raw_surface(
    SDL_Renderer& renderer,
    SDL_Surface& surface) 
{
    return Texture(renderer, surface);
}

Size Texture::size() const 
{
    Size result;
    internal::error_checking::check_function(
        SDL_QueryTexture(
            texture_ptr_.get(), nullptr, nullptr, &result.width,
            &result.height));
    return result;
}

void Texture::draw_to_raw_renderer(SDL_Renderer& raw_renderer, Point position) 
{
    const auto src = create_source_rectangle();
    const auto dst = create_destination_rectangle(position);
    SDL_RenderCopy(&raw_renderer, texture_ptr_.get(), &src, &dst);
}

Texture::Texture(SDL_Renderer& renderer, SDL_Surface& surface)
    : texture_ptr_(SDL_CreateTextureFromSurface(&renderer, &surface)) 
{
    internal::error_checking::check_pointer(texture_ptr_);
}

Surface::Surface(const Filename& filename)
    : surface_ptr_(IMG_Load(filename.c_str())) 
{
    internal::error_checking::check_pointer(surface_ptr_);
}

Surface Surface::load_from_file(const Filename& s) 
{
    return Surface(s);
}

Texture Surface::get_texture_from_raw_renderer(SDL_Renderer& renderer) 
{
    return Texture::from_raw_surface(renderer, *surface_ptr_);
}

Rectangle Texture::create_source_rectangle() const noexcept 
{
    return {0, 0, size().width, size().height};
}

Rectangle Texture::create_destination_rectangle(Point position) const noexcept 
{
    return {position.x, position.y, size().width, size().height};
}

Renderer::Renderer(SDL_Window& window)
    : renderer_ptr_(SDL_CreateRenderer(&window, -1, 0)) 
{
    internal::error_checking::check_pointer(renderer_ptr_);
}

Texture Renderer::load_texture(const Filename& filename) 
{
    auto surface = Surface::load_from_file(filename);
    return surface.get_texture_from_raw_renderer(*renderer_ptr_);
}

void Renderer::set_blend_mode(SDL_BlendMode blend_mode) 
{
    internal::error_checking::check_function(
        SDL_SetRenderDrawBlendMode(renderer_ptr_.get(), blend_mode));
}

void Renderer::render_present() 
{
    SDL_RenderPresent(renderer_ptr_.get());
}

void Renderer::render_clear(Color background_color) 
{
    set_draw_color(background_color);
    internal::error_checking::check_function(
        SDL_RenderClear(renderer_ptr_.get()));
}

void Renderer::draw_texture(Sdl::Texture& texture, Sdl::Point position) 
{
    texture.draw_to_raw_renderer(*renderer_ptr_, position);
}

void Renderer::draw_line(Sdl::Line line, Color line_color) 
{
    set_draw_color(line_color);
    internal::error_checking::check_function(
        SDL_RenderDrawLine(
            renderer_ptr_.get(), line.from.x, line.from.y, line.to.x,
            line.to.y));
}

void Renderer::draw_rectangle(
    Rectangle rect,
    Color rect_color,
    Sdl::Color_filling filling) {
    set_draw_color(rect_color);
    if (filling == Color_filling::filled)
        draw_filled_rectangle(rect);
    else
        draw_empty_rectangle(rect);
}

void Renderer::set_draw_color(Color color) 
{
    SDL_SetRenderDrawColor(
        renderer_ptr_.get(), color.r, color.g, color.b, color.a);
}

void Renderer::draw_filled_rectangle(Rectangle rect) 
{
    internal::error_checking::check_function(
        SDL_RenderFillRect(renderer_ptr_.get(), &rect));
}

void Renderer::draw_empty_rectangle(Rectangle rect) 
{
    internal::error_checking::check_function(
        SDL_RenderDrawRect(renderer_ptr_.get(), &rect));
}

Color Renderer::get_draw_color() const 
{
    Color result;
    SDL_GetRenderDrawColor(
        renderer_ptr_.get(), &result.r, &result.g, &result.b, &result.a);
    return result;
}

const char* Image_cache::Getting_uncached_image::what() const noexcept
{
    return "Image_cache::get_image_texture called for a file not yet cahced."
           "Call Image_cache::cache_image first.";
}

void Image_cache::cache_image(
    const Filename& filename,
    Sdl::Renderer& renderer)
{
    textures_.emplace(
        std::make_pair(filename, renderer.load_texture(filename)));
}

bool Image_cache::image_is_cached(const Filename& filename) const noexcept 
{
    return textures_.count(filename) == 1;
}

Texture& Image_cache::image_texture(const Filename& filename) 
{
    return get_image_texture_impl(*this, filename);
}

const Texture& Image_cache::image_texture(const Filename& filename) const 
{
    return get_image_texture_impl(*this, filename);
}

Window_properties Window_properties::default_values() 
{
    Window_properties properties;
    properties.size.width = 800;
    properties.size.height = 600;
    properties.position.x = properties.position.y = SDL_WINDOWPOS_UNDEFINED;
    return properties;
}

Window_properties Window_properties::default_values_with_title(
    std::string title) 
{
    auto properties = Window_properties::default_values();
    properties.title = std::move(title);
    return properties;
}

Window::Window(const Window_properties& properties, Uint32 flags)
    : window_ptr_(create_checked_window(properties, flags)),
      renderer_(*window_ptr_) {
    renderer_.set_blend_mode(SDL_BLENDMODE_BLEND);
}

void Window::draw_image(const Filename& filename, Point position) {
    cache_image(filename);
    
    const auto draw_image_command = [this, filename, position] {
        auto& texture = image_cache_.image_texture(filename);
        renderer_.draw_texture(texture, position);
    };
    
    draw_commands_.add_command(draw_image_command);
}

void Window::draw_line(Line line, Color color) {
    const auto draw_line_command = [this, line, color] {
        renderer_.draw_line(line, color);
    };
    
    draw_commands_.add_command(draw_line_command);
}

void Window::draw_rectangle(
    Rectangle rect,
    Color color,
    Color_filling filling) {
    const auto draw_rectangle_command = [this, rect, color, filling] {
        renderer_.draw_rectangle(rect, color, filling);
    };
    draw_commands_.add_command(draw_rectangle_command);
}

void Window::redraw(Color background_color) {
    renderer_.render_clear(background_color);
    draw_commands_.execute();
    renderer_.render_present();
}

void Window::Draw_commands::add_command(const Command& command)
{
    draw_commands_.push_back(command);
}

void Window::Draw_commands::execute()
{
    for (const auto& draw_command : draw_commands_)
        draw_command();
    draw_commands_.clear();
}

void Window::cache_image(const Filename& filename) {
    if (!image_cache_.image_is_cached(filename))
        image_cache_.cache_image(filename, renderer_);
}

Unique_window Window::create_checked_window(
    const Window_properties& properties,
    Uint32 flags) 
{
    auto result = create_unchecked_window(properties, flags);
    internal::error_checking::check_pointer(result);
    return result;
}

Unique_window Window::create_unchecked_window(
    const Window_properties& properties,
    Uint32 flags) {
    return Unique_window(
        SDL_CreateWindow(
            properties.title.c_str(), properties.position.x,
            properties.position.y, properties.size.width,
            properties.size.height, flags));
}
}
