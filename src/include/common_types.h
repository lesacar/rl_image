#pragma once
#include <concepts>
#include <type_traits>

namespace engine {
    template <typename T> concept number = std::is_arithmetic_v<T> && !std::same_as<T, bool>;
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

}
