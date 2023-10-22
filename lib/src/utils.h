#pragma once

#include <cstddef>
#include <initializer_list>
#include <utility>

template <typename F, typename T, size_t N, size_t... Idx>
decltype(auto) apply_impl(F &f, T (&t)[N], std::index_sequence<Idx...>) {
    return f(t[Idx]...);
}

/**
 * @brief Applies array values as paramater pack to variadic function.
 * 
 * @param f variadic function.
 * 
 * @param t reference to array.
 */
template <typename F, typename T, std::size_t N>
decltype(auto) apply (F &f, T (&t)[N]) {
    return apply_impl(f, t, std::make_index_sequence<N>{});   
}

/**
 * @brief Fills array by values of specified type.
 *
 * @tparam T value type.
 *
 * @tparam N array capacity.
 *
 * @tparam Values provided values type.
 *
 * @param arr target array.
 *
 * @param values provided values pack.
 */
template <class T, size_t N, typename... Values>
void fill_array(std::array<T, N>& arr, Values... values)
{
    static_assert(N == sizeof...(values));
    size_t j = 0;
    for (auto i : std::initializer_list<std::common_type_t<Values...>>{values...})
    {
        arr[j++] = i;
    }
}