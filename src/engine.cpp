#include <engine.hpp>
#include <raylib.h>

using namespace engine;

class window {
public:
    std::string name;
    vec2<int> size{};
    window(vec2<int> size, std::string_view name) : size(size), name(name)
    {
        pre_initialization();
        InitWindow(size.x, size.y, name.data());
        post_initialization();
    }

    // auto set resolution
    window(std::string_view name) : name(name)
    {
        pre_initialization();
        // init with safe size first
        InitWindow(640, 360, name.data());
        set_best_fit_resolution();
        post_initialization();
    }


    void pre_initialization() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }
    void post_initialization() {
        SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    }

    // set resolution to biggest possible standardized resolution that isn't native (1600x900 on 1080p)
    void set_best_fit_resolution() {
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
    void set_resize_to(vec2<int> newSize) {
        SetWindowSize(newSize.x, newSize.y);
    }
    void center_to_monitor() {
        log(log_level::info, "Centering window to display");
        vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
        SetWindowPosition((max_resolution.x - size.x)/2, (max_resolution.y - size.y)/2);
    }

    // handle anything to do with resizing the window
    void resize_handler() {
        if (IsWindowResized()) {
            size.x = GetScreenWidth();
            size.y = GetScreenHeight();
            // log(log_level::info, "Window was resized: {{{},{}}}", size.x, size.y);
        }
    }

    // getter for global::windowShouldClose
    bool should_close() {
        return global::windowShouldClose;
    }

    // sets global::windowShouldClose to true, make sure to have a main loop that removes this window from the scope when should_close() is true to close the window properly
    void close() {
        global::windowShouldClose = true;
    }

    ~window() {
        log(log_level::info, "Closing window \"{}\"", name);
        CloseWindow();
    }

};

