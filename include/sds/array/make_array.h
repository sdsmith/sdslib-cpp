#pragma once

#include <array>
#include <type_traits>

namespace sds
{
/**
 * \brief Make \a std::array from the given values.
 *
 * Avoids having to specify the number of elements in the array.
 *
 * Usage:
 *  auto arr = make_array(1, 2, 3);
 *  auto arr = make_array<int>(1, 2, 3);
 *
 * ref: https://gist.github.com/klmr/2775736#file-make_array-hpp
 */
template <typename... T>
constexpr auto make_array(T&&... values)
    -> std::array<typename std::decay_t<typename std::common_type<T...>::type>, sizeof...(T)>
{
    return {std::forward<T>(values)...};
}

// Helper
template <typename T, std::size_t N, std::size_t ... Is>
constexpr std::array<T, N>
make_array(const T (&a)[N], std::index_sequence<Is...>)
{
    return {{a[Is]...}};
}
/**
 * \brief Make \a str::array from string literals.
 *
 * Avoids having to specify the number of elements in the array.
 *
 * Usage:
 *  auto arr = make_array("hello world")
 *  auto arr = make_array<char>("hello world")
 *
 * ref: https://stackoverflow.com/a/33484394/3693388
 */
template <typename T, std::size_t N>
constexpr std::array<T, N> make_array(const T (&a)[N])
{
    return make_array(a, std::make_index_sequence<N - 1>());
}

} // namespace sds
