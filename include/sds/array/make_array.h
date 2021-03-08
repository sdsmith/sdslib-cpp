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
 * ref: https://gist.github.com/klmr/2775736#file-make_array-hpp
 */
template <typename... T>
constexpr auto make_array(T&&... values)
    -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
{
    return {std::forward<T>(values)...};
}
} // namespace sds
