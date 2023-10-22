#pragma once

#include <type_traits>

template <typename ...Args>
struct is_integral_variadic : std::true_type {};

template <typename T>
struct is_integral_variadic<T> : std::true_type {};

template <typename T, typename ...Args>
struct is_integral_variadic<T, Args...>
{
    static constexpr bool value = std::is_integral_v<T> && is_integral_variadic<Args...>::value;
};

template <typename ...Args>
inline constexpr bool is_integral_variadic_v = is_integral_variadic<Args...>::value;

template <typename ...Args>
using enable_if_integral_variadic_t = std::enable_if_t<is_integral_variadic_v<Args...>>;
