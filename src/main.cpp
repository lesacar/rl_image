#include "raylib.h"
#include "engine.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <stdlib.h>
#include <string>

namespace global {
    bool windowShouldClose = false;
}


int main() {
    static_assert(sizeof(engine::vec2<float>) == sizeof(Vector2), "raylib Vector2 is not the same as engine::vec2<float>");
    // initialize window with safe resolution
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 360, "Image Viewer");
    engine::vec2 max_resolution{ GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()) };
    if (max_resolution.x == 0) {
        engine::log(engine::log_level::error, "Monitor resolution was 0");
        std::abort();
    }

    engine::vec2<int> windowSize{};

    for (const auto value : engine::various_16_9_resolutions) {
        if (std::abs(max_resolution.x) - std::abs(value.x) > 0 && std::abs(max_resolution.y) - std::abs(value.y) > 0) {
            engine::log(engine::log_level::info, "Found resolution lower then display resolution");
            windowSize.x = value.x;
            windowSize.y = value.y;
            break;
        }
    }

    if (windowSize.x <= 0) {
        engine::log(engine::log_level::error, "Couldn't find any possible resolution");
        std::abort();
    }

    SetWindowPosition((max_resolution.x - windowSize.x)/2, (max_resolution.y - windowSize.y)/2);
    SetWindowSize(windowSize.x, windowSize.y);


    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));


    while (!global::windowShouldClose) {
        BeginDrawing();
        if (IsWindowResized()) {
            windowSize.x = GetScreenWidth();
            windowSize.y = GetScreenHeight();
            engine::log(engine::log_level::info, "Window was resized: {{{},{}}}", windowSize.x, windowSize.y);
        }
       
        ClearBackground(BLACK);
        std::string string_to_draw = "This is a centered test string";
        // default raylib font needs spacing of 2, and DrawText() which uses the default font implicitly has spacing of fontsize/10
        Vector2 string_dim = MeasureTextEx(GetFontDefault(), string_to_draw.c_str(), 20, 2);

        DrawTextEx(GetFontDefault(), string_to_draw.c_str(), Vector2{(windowSize.x - string_dim.x)/2, (windowSize.y - string_dim.y)/2}, 20, 2, RAYWHITE);
        
        EndDrawing();
        if (WindowShouldClose()) {
            global::windowShouldClose = true;
            engine::log(engine::log_level::info, "windowShouldClose: {}", global::windowShouldClose);
        }
    }
    
    CloseWindow();
    return 0;
}
