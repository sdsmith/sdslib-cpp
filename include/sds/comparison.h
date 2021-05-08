#pragma once

namespace sds
{
/**
 * \brief True if elements in both array are equal, otherwise false.
 */
template <typename Input_Iterator1_T, typename Input_Iterator2_T>
constexpr bool equal(Input_Iterator1_T first1, Input_Iterator1_T last1,
                     Input_Iterator2_T first2) noexcept(noexcept(*first1 == *first2))
{
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2) { return false; }
    }
    return true;
}

template <typename T>
constexpr bool equal(T a, T b) noexcept(noexcept(sds::equal(a.first(), a.last(), b.first())))
{
    return sds::equal(a.first(), a.last(), b.first());
}

template <typename Input_Iterator1_T, typename Input_Iterator2_T>
constexpr bool lexicographical_compare(Input_Iterator1_T first1, Input_Iterator1_T last1,
                                       Input_Iterator2_T first2,
                                       Input_Iterator2_T last2) /* TODO(sdsmith): noexcept */
{
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
        if (*first1 < *first2) { return true; }
        if (*first2 < *first1) { return false; }
    }
    return (first1 == last1) && (first2 == last2);
}

template <typename T>
constexpr bool lexicographical_compare(T a, T b) /* TODO(sdsmith): noexcept */
{
    return sds::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

} // namespace sds
