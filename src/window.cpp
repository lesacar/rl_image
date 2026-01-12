#include "window.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cstdlib>
#include <engine.hpp>
#include <cstring>

// Window resize constants
constexpr int DISPLAY_MARGIN = 100;          // Margin from screen edges
constexpr engine::vec2<int> MIN_WINDOW_SIZE{400, 240};

namespace engine {
    window::window(vec2<int> size, std::string_view name) : name(name), size(size)
    {
        // memset(&cam, 0, sizeof(cam));
        pre_initialization();
        InitWindow(size.x, size.y, name.data());
        post_initialization();
    }

    // auto set resolution
    window::window(std::string_view name) : name(name), size{640, 360}
    {
        // memset(&cam, 0, sizeof(cam));
        pre_initialization();
        InitWindow(640, 360, name.data());
        set_best_fit_resolution();
        post_initialization();
    }


    void window::pre_initialization() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_VSYNC_HINT);
    }
    void window::post_initialization() {
        SetExitKey(KEY_NULL);
        // no need for targetFPS if we only use VSync on/off instead
        // SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
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
            size = {640, 360};
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

            // Vector2 old_size = Vector2{static_cast<float>(size.x),static_cast<float>(size.y)};
            size.x = GetScreenWidth();
            size.y = GetScreenHeight();
            // Vector2 old_target = cam.target;
            // Vector2 new_size = Vector2{static_cast<float>(size.x),static_cast<float>(size.y)};
            // Vector2 size_diff = Vector2Subtract(new_size, old_size);
            // cam.target = Vector2Subtract(cam.target, Vector2Scale(size_diff, 0.5f / cam.zoom));
            cam.offset = Vector2{size.x * 0.5f, size.y * 0.5f};

            // log(log_level::info, "Window was resized: {{{},{}}}", size.x, size.y);
            // log(log_level::info, "New camera target: {{{},{}}}", cam.target.x, cam.target.y);
            // log(log_level::info, "New camera offset: {{{},{}}}", cam.offset.x, cam.offset.y);
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
            auto _argc = cli_args.size();
            auto _argv1 = cli_args.at(1);
            log(log_level::info, "argc: {} argv: {}", _argc, _argv1);
        }
    }

    // getter for global::windowShouldClose, also checks if any closing event happened and initiates shutdown if it did
    bool window::should_close() {
        if (WindowShouldClose()) {
            global::windowShouldClose = true;
        }
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


    bool window::toggle_show_fps() {
        show_fps = !show_fps;
        return show_fps;
    }

    bool window::toggle_show_fps(bool new_show_fps) {
        show_fps =  new_show_fps;
        return show_fps;
    }

    bool window::resize_to_fit_image(const Image& img) {
        if (!IsImageValid(img)) {
            engine::log(engine::log_level::error, "Cannot resize window to invalid image");
            return false;
        }

        engine::log(engine::log_level::info, "Image dimensions: {}x{}", img.width, img.height);

        // Get display bounds
        engine::vec2<int> display_size{
            GetMonitorWidth(GetCurrentMonitor()),
            GetMonitorHeight(GetCurrentMonitor())
        };

        // Calculate maximum window size (display minus margin)
        const int max_window_width = display_size.x - DISPLAY_MARGIN;
        const int max_window_height = display_size.y - DISPLAY_MARGIN;

        // Calculate scale to fit image in available window area
        float width_ratio = static_cast<float>(max_window_width) / img.width;
        float height_ratio = static_cast<float>(max_window_height) / img.height;
        float scale = std::min(width_ratio, height_ratio);

        // Clamp scale: maximum 1.0 (pixel-perfect), minimum 0.1
        scale = std::clamp(scale, 0.1f, 1.0f);

        // Calculate window size needed (image × scale)
        engine::vec2<int> window_size{
            static_cast<int>(img.width * scale),
            static_cast<int>(img.height * scale)
        };

        // Clamp to minimum window size
        window_size.x = std::max(window_size.x, MIN_WINDOW_SIZE.x);
        window_size.y = std::max(window_size.y, MIN_WINDOW_SIZE.y);

        engine::log(engine::log_level::info,
                   "Scale: {:.2f}, Window size: {}x{}",
                   scale, window_size.x, window_size.y);

        // Get current window size before resize
        engine::vec2<int> current_size{GetScreenWidth(), GetScreenHeight()};
        engine::log(engine::log_level::info, "Current render area: {}x{}",
                   current_size.x, current_size.y);

        // Resize window
        SetWindowSize(window_size.x, window_size.y);
        size = window_size;

        // Get new render area size (should match our calculated render_size)
        engine::vec2<int> new_render_size{GetScreenWidth(), GetScreenHeight()};

        // Center on monitor
        center_to_monitor();

        // Reset camera for new image with calculated scale
        cam.target = Vector2Zero();
        cam.zoom = scale;
        // Update camera offset immediately for new window size
        // Use actual render area size (GetScreenWidth/Height) not calculated window_size
        cam.offset = Vector2{static_cast<float>(new_render_size.x) * 0.5f,
                            static_cast<float>(new_render_size.y) * 0.5f};

        engine::log(engine::log_level::info,
                   "Window resized to: {}x{}, render area: {}x{} (image: {}x{}), camera zoom: {:.2f}",
                   window_size.x, window_size.y,
                   new_render_size.x, new_render_size.y,
                   img.width, img.height, scale);

        return true;
    }
}
