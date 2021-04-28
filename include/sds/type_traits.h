#pragma once

#include "sds/details/common.h"
#include <type_traits>

#if SDS_USE_RTTI_FEATURES
#    include <typeinfo>
#endif

namespace sds
{
/**
 * \brief Check if type \a T is contained in the type list \a Ts.
 *
 * Usage: `contains<int, Args...>::value` or `contains<int, char, bool, int>::value`.
 */
template <typename T, typename... Ts>
struct contains : std::disjunction<std::is_same<T, Ts>...> {};

/**
 * \brief Helper for contains.
 *
 * \see contains
 */
template <typename T>
inline constexpr bool contains_v = contains<T>::value;

#if SDS_USE_RTTI_FEATURES
template <typename T>
char const* type_name()
{
    return typeid(T).name();
}

#    define SDS_TYPE_NAME(x) type_name<decltype(x)>()
#endif
} // namespace sds
