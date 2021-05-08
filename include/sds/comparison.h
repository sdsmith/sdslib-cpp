#pragma once

namespace sds
{
/**
 * \brief True if elements in both array are equal, otherwise false.
 */
template <typename Input_Iterator1_T, typename Input_Iterator2_T>
constexpr bool equal(Input_Iterator1_T begin1, Input_Iterator1_T end1,
                     Input_Iterator2_T begin2) noexcept(noexcept(*begin1 == *begin2))
{
    for (; begin1 != end1; ++begin1, ++begin2) {
        if (*begin1 != *begin2) { return false; }
    }
    return true;
}

template <typename T>
constexpr bool equal(T a, T b) noexcept(noexcept(sds::equal(a.begin(), a.end(), b.begin())))
{
    return sds::equal(a.begin(), a.end(), b.begin());
}

} // namespace sds
