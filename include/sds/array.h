#pragma once

#include "sds/comparison.h"
#include "sds/details/common.h"
#include "sds/iterator.h"
#include <type_traits>

#if SDS_INCLUDE_CPPSTD_FEATURES
#    include <array>
#endif

namespace sds
{
#if SDS_INCLUDE_CPPSTD_FEATURES
/**
 * \brief Return the size of the given array in bytes.
 */
template <typename T, size_t Size>
constexpr size_t byte_size(std::array<T, Size> const&)
{
    return Size * sizeof(T);
}
#endif

/**
 * \brief Array implementation built ontop of a c-array.
 *
 * Interface is compatible with \a std::array.
 *
 * \tparam T Array element type.
 * \tparam N Length.
 */
template <typename T, sz N>
class Array {
    SDS_STATIC_ASSERT(N > 0);

public:
    /*
     * Implementation tries to reduce function call nesting to reduce callstack length.
     */

    /* NOTE(sdsmith): This is intentionally left public to allow for aggregate initialization.
     * Example usage:
     *     Array<int, 3> a = {{1,2,3}}; // Strict compilers require double brackets
     */
    T m_arr[N];

    /* NOTE(sdsmith): For C++ standard library compatibility. */
    using value_type = T;
    using size_type = sz;
    using difference_type = ptrdiff;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = value_type*;
    using const_iterator = value_type const*;
    using reverse_iterator = sds::Reverse_Iterator<iterator>;
    using const_reverse_iterator = sds::Reverse_Iterator<const_iterator>;

    Array() = default;
    Array(Array<T, N> const&) = delete;
    Array(Array<T, N>&&) = delete;

    constexpr void fill(T v) noexcept(std::is_nothrow_constructible_v<T>);
    constexpr void swap(Array<T, N>& o) noexcept(std::is_nothrow_swappable_v<value_type>);

    [[nodiscard]] constexpr reference operator[](sz pos) noexcept { return m_arr[pos]; }
    [[nodiscard]] constexpr const_reference operator[](sz pos) const noexcept
    {
        SDS_ASSERT(pos < N);
        return m_arr[pos];
    }

    [[nodiscard]] constexpr reference front() noexcept { return m_arr[0]; }
    [[nodiscard]] constexpr const_reference front() const noexcept { return m_arr[0]; }

    [[nodiscard]] constexpr reference back() noexcept { return m_arr[N - 1]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return m_arr[N - 1]; }

    [[nodiscard]] constexpr pointer data() noexcept { return m_arr; }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return m_arr; }

    [[nodiscard]] constexpr bool empty() const noexcept { return false; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }
    [[nodiscard]] constexpr size_type size() const noexcept { return max_size(); }

    [[nodiscard]] constexpr iterator begin() noexcept { return &m_arr[0]; }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return &m_arr[0]; }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return &m_arr[0]; }

    [[nodiscard]] constexpr iterator end() noexcept { return &m_arr[N]; }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return &m_arr[N]; }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return &m_arr[N]; }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return {&m_arr[N]}; }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return {&m_arr[N]}; }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return {&m_arr[N]}; }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept { return {&m_arr[0]}; }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return {&m_arr[0]}; }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return {&m_arr[0]}; }

    Array<T, N> operator=(Array<T, N> const&) = delete;
    Array<T, N> operator=(Array<T, N>&&) = delete;
};

template <typename T, sz N>
constexpr void Array<T, N>::fill(T v) noexcept(std::is_nothrow_constructible_v<T>)
{
    for (sz i = 0; i < N; ++i) { m_arr[i] = v; }
}

template <typename T, sz N>
constexpr void Array<T, N>::swap(Array<T, N>& o) noexcept(std::is_nothrow_swappable_v<value_type>)
{
    sds::swap_ranges(&m_arr[0], &m_arr[N], &o.m_arr[0]);
}

template <typename T, sz N>
constexpr bool operator==(Array<T, N> const& a,
                          Array<T, N> const& b) noexcept(noexcept(sds::equal(a, b)))
{
    return sds::equal(a, b);
}

template <typename T, sz N>
constexpr bool operator!=(Array<T, N> const& a, Array<T, N> const& b) noexcept(noexcept(a == b))
{
    return !(a == b);
}

template <typename T, sz N>
constexpr bool operator<(Array<T, N> const& a,
                         Array<T, N> const& b) noexcept(noexcept(sds::lexicographical_compare(a,
                                                                                              b)))
{
    return sds::lexicographical_compare(a, b);
}

template <typename T, sz N>
constexpr bool operator<=(Array<T, N> const& a, Array<T, N> const& b) noexcept(noexcept(a > b))
{
    return !(a > b);
}

template <typename T, sz N>
constexpr bool operator>(Array<T, N> const& a,
                         Array<T, N> const& b) noexcept(noexcept(sds::lexicographical_compare(b,
                                                                                              a)))
{
    return sds::lexicographical_compare(b, a);
}

template <typename T, sz N>
constexpr bool operator>=(Array<T, N> const& a, Array<T, N> const& b) noexcept(noexcept(a > b))
{
    return !(a < b);
}

/**
 * \brief Return the size of the given array in bytes.
 */
template <typename T, sz Size>
constexpr size_t byte_size(sds::Array<T, Size> const&)
{
    return Size * sizeof(T);
}
} // namespace sds
