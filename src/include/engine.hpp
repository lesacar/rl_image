#pragma once
#include <iostream>
#include <ostream>
#include <print>
#include <raylib.h>
#include <string_view>
#include <array>
#include <common_types.h>
#include <window.hpp>
#include <wimage.hpp>

// global variables that will (potentially) be read by every single file
// make sure to INLINE everything, 2 different translation units NEED to have the same variable
namespace global {
    inline bool windowShouldClose = false;
    inline std::array<std::string_view, 2> image_filetypes = { ".png", ".jpg" };
}

namespace engine {
    // this should always be descending, since it's used by an algorithm that expects sorted values
    inline constexpr std::array<vec2<int>, 6> various_16_9_resolutions =
    {{
         {2560,1440},
         {1920,1080},
         {1600,900},
         {1280,720},
         {960,540},
         {640,360},
     }};


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
            std::println("{}{}{}\033[0m", engine_name, level_str, std::vformat(str, std::make_format_args(Args(args)...)));
            std::flush(std::cout);
        }

    bool is_supported_image_extension(std::string_view img_path);
    [[nodiscard("\'Tried to discard image\'")]] Image image_was_provided(window& w);

    void DrawTextureMidpoint(Texture2D tex, Vector2 point, float rotation);

}   // namespace engine
