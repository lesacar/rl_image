#pragma once
#include <string_view>
#include <print>
#include <array>


namespace engine {

    template <typename T>
    struct vec2 {
        T x;
        T y;

        constexpr vec2(T x, T y) : x(x), y(y) {};

        // cast any type U to any vector<T> type if the cast is defined (should work for normal things like ints, floats, etc...)
        // for custom classes you need to implement custom casting to anything you want to cast to
        template <typename U>
        constexpr vec2(const vec2<U>& u) : x(static_cast<T>(u.x)) , y(static_cast<T>(u.y)) {}

        // if no arguments passed, zero initialize the vector with corresponding type
        constexpr vec2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {};


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
