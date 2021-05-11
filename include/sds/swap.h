#pragma once

#include "sds/move.h"
#include <type_traits>

namespace sds
{
template <typename T>
constexpr void swap(T& a, T& b) noexcept(
    std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>)
{
    T tmp(sds::move(a));
    a = sds::move(b);
    b = sds::move(tmp);
}

template <typename Forward_It1, typename Forward_It2>
constexpr void iter_swap(Forward_It1 a,
                         Forward_It2 b) // TODO(sdsmith): noexcept
{
    sds::swap(*a, *b);
}

template <typename Forward_It1, typename Forward_It2>
constexpr Forward_It2 swap_ranges(Forward_It1 first1, Forward_It1 last1,
                                  Forward_It2 first2) // TODO(sdsmith): noexcept
{
    while (first1 != last1) { sds::iter_swap(first1++, first2++); }
    return first2;
}
} // namespace sds
