#pragma once

#include <type_traits>

namespace sds {

/**
 * \brief Check if type \a T is contained in the type list \a Ts.
 *
 * Usage: `contains<int, Args...>::value` or `contains<int, char, bool, int>::value`.
 */
template<typename T, typename... Ts>
struct contains : std::disjunction<std::is_same<T, Ts>...> {};

} // namespace sds
