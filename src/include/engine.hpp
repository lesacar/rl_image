#pragma once
#include <cstddef>
#include <string>
#include <print>
#include <raylib.h>
#include <string_view>
#include <array>
#include <vector>

// global variables that will (potentially) be read by every single file
// make sure to INLINE everything, 2 different translation units NEED to have the same variable
namespace global {
    inline bool windowShouldClose = false;
    inline std::array<std::string_view, 2> image_filetypes = { ".png", ".jpg" };
}

namespace engine {

    template <typename T> concept number = std::is_arithmetic_v<T> && !std::same_as<std::remove_cv_t<T>, bool>;
    // number is now any arithmetic type except bool (int, float, size_t, double, uint8_t, ...)
    template <number T>
        struct vec2 {
            T x{};
            T y{};
            // will generate a constructor that calls the above x{} and y{} value zero-initializtion
            constexpr vec2() noexcept = default;
            // self-explanatory
            constexpr vec2(T x, T y) noexcept : x(x), y(y) {}
            // allow type-safe casting to other number types (vec2<float> v = static_cast<float>vec2<int>{1,2})
            template <number U>
                explicit constexpr vec2(const vec2<U>& u) noexcept : x(static_cast<T>(u.x)) , y(static_cast<T>(u.y)) {}
        };

    class window {
        private:
            std::vector<std::string_view> cli_args;
        public:
            std::string name;
            engine::vec2<int> size{};

            window() = delete;
            window(engine::vec2<int> size, std::string_view name);
            window(std::string_view name);
            ~window();

            void pre_initialization();
            std::vector<std::string_view>& get_cli_args();
            void post_initialization();
            void set_best_fit_resolution();
            void set_resize_to(engine::vec2<int> newSize);
            void center_to_monitor();
            void resize_handler();

            // pass argc and argv into the window
            void append_cli_args(size_t argc, const char* argv[]);

            bool should_close();
            void close();
    };

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
            std::println("{}{}{}\033[0m", engine_name, level_str, std::vformat(str, std::make_format_args(std::forward<Args>(args)...)));
        }

    bool is_supported_image_extension(std::string_view img_path);
    [[nodiscard("\'Tried to discard image\'")]] Image image_was_provided(window& w);

    class working_image {
        private:
            Image img;
            Texture2D img_tex;
        public:
            Texture2D get_tex();
            // after modifying the image externaly (e.g. rotating), save the updated version, crash on fail
            void set_image(Image image);
            working_image() = delete;
            working_image(engine::window& w);
            ~working_image();
    };

    // namespace engine
}
