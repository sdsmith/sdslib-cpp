#pragma once

#include <type_traits>

namespace sds
{
/**
 * \brief Move. Equivalent to std::move.
 *
 * ref: https://en.cppreference.com/w/cpp/utility/move
 */
template <typename T>
typename std::remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference_t<T>&&>(t);
}
} // namespace sds
