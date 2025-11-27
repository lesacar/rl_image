#include "raylib.h"
#include <engine.hpp>
#include <cstdlib>
#include <string>
#include <string_view>

class window {
public:
    std::string name;
    engine::vec2<int> size{};
    window(engine::vec2<int> size, std::string_view name) : size(size), name(name)
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
        engine::vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
        if (max_resolution.x <= 0 || max_resolution.y <= 0) {
            engine::log(engine::log_level::error, "Monitor did not share it's resolution.");
            std::abort();
        }

        // find the best possible non fullscreen resolution (1600x900 on 1080p)
        for (const auto value : engine::various_16_9_resolutions) {
            if (std::abs(max_resolution.x) - std::abs(value.x) > 0 && std::abs(max_resolution.y) - std::abs(value.y) > 0) {
                // TODO: implement print formatter for engine::vec2
                engine::log(engine::log_level::info, "Found resolution ({}x{}) lower then display resolution ({}x{})", value.x, value.y, max_resolution.x, max_resolution.y);
                size.x = value.x;
                size.y = value.y;
                break;
            }
        }

        if (size.x <= 0 || size.y <= 0) {
            engine::log(engine::log_level::error, "Couldn't find any possible resolution");
            std::abort();
        }

        center_to_monitor();
        SetWindowSize(size.x, size.y);
    }
    void set_resize_to(engine::vec2<int> newSize) {
        SetWindowSize(newSize.x, newSize.y);
    }
    void center_to_monitor() {
        engine::log(engine::log_level::info, "Centering window to display");
        engine::vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
        SetWindowPosition((max_resolution.x - size.x)/2, (max_resolution.y - size.y)/2);
    }

    // handle anything to do with resizing the window
    void resize_handler() {
        if (IsWindowResized()) {
            size.x = GetScreenWidth();
            size.y = GetScreenHeight();
            // engine::log(engine::log_level::info, "Window was resized: {{{},{}}}", size.x, size.y);
        }
    }

    bool should_close() {
        if (global::windowShouldClose) {
            return true;
        }
        return false;
    }
    void close() {
        global::windowShouldClose = true;
    }

    ~window() {
        engine::log(engine::log_level::info, "Closing window \"{}\"", name);
        CloseWindow();
    }

};

int main() {
    window window{"Image Viewer"};
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");

    while (!window.should_close()) {
        BeginDrawing();
        window.resize_handler();
       
        ClearBackground(BLACK);
        std::string string_to_draw = "This is a centered test string";
        // default raylib font needs spacing of 2, and DrawText() which uses the default font implicitly has spacing of fontsize/10
        Vector2 string_dim = MeasureTextEx(GetFontDefault(), string_to_draw.c_str(), 20, 2);

        DrawTextEx(GetFontDefault(), string_to_draw.c_str(), Vector2{(window.size.x - string_dim.x)/2, (window.size.y - string_dim.y)/2}, 20, 2, RAYWHITE);
        
        EndDrawing();
        if (WindowShouldClose()) {
            window.close();
            // engine::log(engine::log_level::info, "windowShouldClose: {}", global::windowShouldClose);
        }
    }
    
    return 0;
}
