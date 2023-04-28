#pragma once

#include "sds/details/common.h"

#include "sds/array/array.h"
#include "sds/bit.h"
#include "sds/string.h"
#include <initializer_list>
#include <limits>
#include <optional>

namespace sds
{
/*
 * \brief Contiguous bit array.
 *
 * Similar to std::bitset with stronger guarantees: the bits are always contiguous.
 *
 * \tparam N Number of bits.
 */
template <sz N>
class Bitarray {
    static constexpr s32 s_num_trailing_bits = N % sds::bit_size<u32>();
    static constexpr s32 s_arr_els = sds::bits_fit_in_num_elements<u32>(N);
    sds::Array<u32, s_arr_els> m_arr{0};

    void fill(bool v) noexcept;

public:
    Bitarray() = default;
    /**
     * \brief Construct Bitarray from the given values.
     *
     * \param bits Interpreted as [[bit31..bit0], [bit63..bit32], ...].
     *
     */
    constexpr Bitarray(std::initializer_list<u32> bits);

    /**
     * @brief Construct Bitarray from the given values. Interpreted as [bit0, bit1, bit2, ...].
     *
     */
    Bitarray(std::initializer_list<bool> bits);

    [[nodiscard]] bool all() const noexcept;
    [[nodiscard]] bool any() const noexcept;
    [[nodiscard]] bool none() const noexcept;

    [[nodiscard]] std::optional<sz> first_unset() const noexcept;

    class Bit {
        Array<u32, s_arr_els>* m_arr;
        s32 m_index = 0;
        s32 m_shift = 0;

    public:
        Bit(Array<u32, s_arr_els>& arr, s32 index, s32 shift)
            : m_arr(&arr), m_index(index), m_shift(shift)
        {}

        Bit& operator=(bool v) noexcept
        {
            if (v) {
                (*m_arr)[m_index] |= 1U << m_shift;
            } else {
                (*m_arr)[m_index] &= sds::bitmask<32>() ^ (1U << m_shift);
            }
            return *this;
        }

        void flip(s32 pos) noexcept { (*m_arr)[m_index] ^= 1U << m_shift; }
    };

    [[nodiscard]] Bit operator[](s32 pos) noexcept;
    [[nodiscard]] bool operator[](s32 pos) const noexcept;
    [[nodiscard]] bool test(s32 pos) const noexcept;
    [[nodiscard]] s32 count() const noexcept;

    /**
     * @brief Set (to 1) given bit.
     *
     * @param pos
     */
    void set(s32 pos) noexcept;
    /**
     * @brief Reset (to 0) given bit.
     *
     * @param pos
     */
    void reset(s32 pos) noexcept;
    void flip(s32 pos) noexcept;

    void set_all() noexcept { fill(true); }
    void reset_all() noexcept { fill(false); }

    [[nodiscard]] constexpr sz size() const noexcept { return N; }

    Bitarray<N>& operator&=(Bitarray<N> const& o) noexcept;
    Bitarray<N>& operator|=(Bitarray<N> const& o) noexcept;
    Bitarray<N>& operator^=(Bitarray<N> const& o) noexcept;
    Bitarray<N> operator~() const noexcept;

    /* TODO:
    Bitarray<N> operator<<(sz n) const noexcept;
    Bitarray<N>& operator<<=(sz n) noexcept;
    Bitarray<N> operator>>(sz n) const noexcept;
    Bitarray<N>& operator>>=(sz n) noexcept;
    */

    [[nodiscard]] bool operator==(Bitarray<N> const& o) const noexcept;
    [[nodiscard]] bool operator!=(Bitarray<N> const& o) const noexcept;

    [[nodiscard]] sds::String to_string() const noexcept;
};

template <sz N>
void Bitarray<N>::fill(bool v) noexcept
{
    m_arr.fill(v ? ~0U : 0U);

    // Maintain 0s in unused bits
    m_arr.back() &= sds::bitmask<s_num_trailing_bits>();
}

template <sz N>
constexpr Bitarray<N>::Bitarray(std::initializer_list<u32> bits) {
    SDS_ASSERT(bits.size() < std::numeric_limits<s32>::max());
    s32 i = 0;
    for (u32 e : bits) {
        m_arr[i] = e;
        ++i;
    }
}

template <sz N>
Bitarray<N>::Bitarray(std::initializer_list<bool> bits) {
    SDS_ASSERT(bits.size() <= std::numeric_limits<s32>::max());
    s32 i = 0;
    for (bool bit : bits) {
        bit ? set(i) : reset(i);
        ++i;
    }
}

template <sz N>
bool Bitarray<N>::all() const noexcept
{
    sz size = m_arr.size();
    for (sz i = 0; i < size - 1; ++i) {
        if (m_arr[i] != ~0U) { return false; }
    }

    return m_arr.back() == sds::bitmask<s_num_trailing_bits>();
}

template <sz N>
bool Bitarray<N>::any() const noexcept
{
    for (u32 e : m_arr) {
        if (e != 0) { return true; }
    }
    return false;
}

template <sz N>
bool Bitarray<N>::none() const noexcept
{
    return !any();
}

template <sz N>
std::optional<sz> Bitarray<N>::first_unset() const noexcept
{
    // TODO(sdsmith): @perf: find first set optimization

    s32 i = 0;
    while (i < N) {
        s32 el_idx = i / sds::bit_size<u32>();
        s32 shift = i % sds::bit_size<u32>();
        if ((m_arr[el_idx] & (1 << shift)) == 0) { return i; }
        ++i;
    }
    return {};
}

template <sz N>
typename Bitarray<N>::Bit Bitarray<N>::operator[](s32 pos) noexcept
{
    return Bit(m_arr, pos / sds::bit_size<u32>(), pos % sds::bit_size<u32>());
}

template <sz N>
bool Bitarray<N>::operator[](s32 pos) const noexcept
{
    return m_arr[pos / sds::bit_size<u32>()] & (1U << (pos % sds::bit_size<u32>()));
}

template <sz N>
bool Bitarray<N>::test(s32 pos) const noexcept
{
    return operator[](pos);
}

template <sz N>
s32 Bitarray<N>::count() const noexcept
{
    s32 count = 0;
    for (u32 e : m_arr) { count += sds::bit_count(e); }
    return count;
}

template <sz N>
void Bitarray<N>::set(s32 pos) noexcept
{
    SDS_ASSERT(pos < N);
    (*this)[pos] = true;
}

template <sz N>
void Bitarray<N>::reset(s32 pos) noexcept
{
    SDS_ASSERT(pos < N);
    (*this)[pos] = false;
}

template <sz N>
void Bitarray<N>::flip(s32 pos) noexcept
{
    SDS_ASSERT(pos < N);
    Bit b = (*this)[pos];
    b.flip();
}

template <sz N>
Bitarray<N>& Bitarray<N>::operator&=(Bitarray<N> const& o) noexcept
{
    sz size = m_arr.size();
    for (sz i = 0; i < size; ++i) { m_arr[i] &= o.m_arr[i]; }
}

template <sz N>
Bitarray<N>& Bitarray<N>::operator|=(Bitarray<N> const& o) noexcept
{
    sz size = m_arr.size();
    for (sz i = 0; i < size; ++i) { m_arr[i] |= o.m_arr[i]; }
}

template <sz N>
Bitarray<N>& Bitarray<N>::operator^=(Bitarray<N> const& o) noexcept
{
    sz size = m_arr.size();
    for (sz i = 0; i < size; ++i) { m_arr[i] ^= o.m_arr[i]; }
}

template <sz N>
Bitarray<N> Bitarray<N>::operator~() const noexcept
{
    Bitarray<N> a = *this;
    for (u32& e : a.m_arr) { e = ~e; }

    // Maintain 0s in unused bits
    a.m_arr.back() &= sds::bitmask<s_num_trailing_bits>();
}

/* TODO(sdsmith):
template <sz N>
Bitarray<N> Bitarray<N>::operator<<(sz n) const noexcept;
template <sz N>
Bitarray<N>& Bitarray<N>::operator<<=(sz n) noexcept;
template <sz N>
Bitarray<N> Bitarray<N>::operator>>(sz n) const noexcept;
template <sz N>
Bitarray<N>& Bitarray<N>::operator>>=(sz n) noexcept;
*/

template <sz N>
bool Bitarray<N>::operator==(Bitarray<N> const& o) const noexcept
{
    if (o.size() != this->size()) { return false; }

    sz size = m_arr.size();
    for (sz i = 0; i < size; ++i) {
        if (m_arr[i] != o.arr[i]) { return false; }
    }
    return true;
}

template <sz N>
bool Bitarray<N>::operator!=(Bitarray<N> const& o) const noexcept
{
    if (o.size() != this->size()) { return true; }

    sz size = m_arr.size();
    for (sz i = 0; i < size; ++i) {
        if (m_arr[i] == o.arr[i]) { return false; }
    }
    return true;
}

template <sz N>
sds::String Bitarray<N>::to_string() const noexcept
{
    // NOTE(sdsmith): in each element, the bits are stored from msb to lsb. ex:
    //   [31..0][63..32][95..64] and so on.
    // This isn't ideal for display, and it is currently not swizzled for the user.

    sz const size = m_arr.size();

    sds::String s;
    s.reserve(size * sds::bit_size<decltype(m_arr)::value_type>());
    for (sz i = 0; i < size; ++i) { s += ((*this)[i] ? '1' : '0'); }

    return s;
}
} // namespace sds
