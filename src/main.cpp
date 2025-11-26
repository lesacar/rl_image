#include "raylib.h"
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <utility>

namespace global {
    bool windowShouldClose = false;
}

namespace engine {
    // will be prepended to engine::log output
    enum class log_level {
        info,
        warning,
        error
    };

    // behaves like std::format (C++23)
    // engine::log(engine::log_level::info, "some info");
    // engine::log(engine::log_level::info, "some info about {}", x);
    template<typename... Args>
    void log(log_level level, const std::string_view str, Args&&... args) {

        const std::string_view engine_name = "ENGINE";
        switch (level) {
            case log_level::info:    std::print("\033[90m"); break;
            case log_level::warning: std::print("\033[93m"); break;
            case log_level::error:   std::print("\033[91m"); break;
        }

        std::string_view level_str =
            level == log_level::info    ? ": INFO: " :
            level == log_level::warning ? ": WARNING: " :
            ": ERROR: ";
        std::println("{}{}{}\033[0m", engine_name, level_str, std::vformat(str, std::make_format_args(std::forward<Args>(args)...)));
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "RL Image Project");
    SetTargetFPS(60);


    while (!global::windowShouldClose) {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        DrawText("Raylib C++ template", 190, 200, 20, DARKGRAY);
        
        EndDrawing();
        if (WindowShouldClose()) {
            global::windowShouldClose = true;
            engine::log(engine::log_level::info, "global variable has changed to {}", global::windowShouldClose);
        }
    }
    
    CloseWindow();
    return 0;
}
