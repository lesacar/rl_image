#include <cstddef>
#include <engine.hpp>
#include <raylib.h>
#include <string_view>
#include <tuple>

using namespace engine;

std::string name;
vec2<int> size{};
window::window(vec2<int> size, std::string_view name) : size(size), name(name)
{
    pre_initialization();
    InitWindow(size.x, size.y, name.data());
    post_initialization();
}

// auto set resolution
window::window(std::string_view name) : name(name)
{
    pre_initialization();
    // init with safe size first
    InitWindow(640, 360, name.data());
    set_best_fit_resolution();
    post_initialization();
}


void window::pre_initialization() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
}
void window::post_initialization() {
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
}

// set resolution to biggest possible standardized resolution that isn't native (1600x900 on 1080p)
void window::set_best_fit_resolution() {
    vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
    if (max_resolution.x <= 0 || max_resolution.y <= 0) {
        log(log_level::error, "Monitor did not share it's resolution.");
        std::abort();
    }

    // find the best possible non fullscreen resolution (1600x900 on 1080p)
    for (const auto value : various_16_9_resolutions) {
        if (std::abs(max_resolution.x) - std::abs(value.x) > 0 && std::abs(max_resolution.y) - std::abs(value.y) > 0) {
            // TODO: implement print formatter for vec2
            log(log_level::info, "Found resolution ({}x{}) lower then display resolution ({}x{})", value.x, value.y, max_resolution.x, max_resolution.y);
            size.x = value.x;
            size.y = value.y;
            break;
        }
    }

    if (size.x <= 0 || size.y <= 0) {
        log(log_level::error, "Couldn't find any possible resolution");
        std::abort();
    }

    center_to_monitor();
    SetWindowSize(size.x, size.y);
}
void window::set_resize_to(vec2<int> newSize) {
    SetWindowSize(newSize.x, newSize.y);
}
void window::center_to_monitor() {
    log(log_level::info, "Centering window to display");
    vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
    SetWindowPosition((max_resolution.x - size.x)/2, (max_resolution.y - size.y)/2);
}

// handle anything to do with resizing the window
void window::resize_handler() {
    if (IsWindowResized()) {
        size.x = GetScreenWidth();
        size.y = GetScreenHeight();
        // log(log_level::info, "Window was resized: {{{},{}}}", size.x, size.y);
    }
}

void window::append_cli_args(size_t argc, const char* argv[])
{
    for (size_t i = 0; i < argc; i++) {
        cli_args.emplace_back(argv[i]);
    }
    if (cli_args.size() > 1) {
        // have to do it like this because MSVC can't handle an rvalue and lvalue& in the same make_format_args call
        // cli_args.size() is a rvalue (size_t)  
        // cli_args.at(1) is a lvalue reference (std::string_view&)
        auto argc = cli_args.size();
        auto argv1 = cli_args.at(1);
        log(log_level::info, "argc: {} argv: {}", argc, argv1);
    }
}

// getter for global::windowShouldClose
bool window::should_close() {
    return global::windowShouldClose;
}

std::vector<std::string_view>& window::get_cli_args()
{
    return cli_args;
}

// sets global::windowShouldClose to true, make sure to have a main loop that removes this window from the scope when should_close() is true to close the window properly
void window::close() {
    global::windowShouldClose = true;
}

window::~window() {
    log(log_level::info, "Closing window \"{}\"", name);
    CloseWindow();
}


