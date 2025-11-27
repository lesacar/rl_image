#pragma once
#include <string_view>
#include <print>
#include <array>
#include <type_traits>


namespace engine {

    template <typename T>
        // only allow intereger or fractional numbers (exclude bool from arithmetic pool)
        concept number = std::is_arithmetic_v<T> && !std::same_as<std::remove_cv_t<T>, bool>;
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

    // this should always be descending, since it's used by an algorithm that expects sorted values
    constexpr std::array<vec2<int>, 5> various_16_9_resolutions{{
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
}
