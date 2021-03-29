#pragma once

#include "sds/bit.h"
#include "sds/details/common.h"
#include <array>

namespace sds {
template<s32 Bit_Size>
class Bitfield { // TODO(sdsmith): bitflags more appropriate?
    static_assert(Bit_Size > 0, "invalid size");

public:
    using underlying_type = s32;

    Bitfield() = default;

    [[nodiscard]] bool empty() const noexcept {
        s32 i = 0;
        while (i < m_data.size() && !m_data[i]) { ++i; }
        return i == m_data.size();
    }

    [[nodiscard]] bool full() const noexcept {
        constexpr u32 last_el_mask = sds::bitmask<s_bit_size % s_underlying_type_bit_size>();
        if ((m_data.back() & last_el_mask) != last_el_mask) { return false; }

        s32 const size = m_data.size() - 1; // -1: already checked last el
        s32 i = 0;
        u32 constexpr mask = ~0U;
        while (i < size && !(m_data[i] ^ mask)) { ++i; }
        return i == size;
    }

    [[nodiscard]] s32 first_unset() const noexcept {
        s32 i = 0;

        // TODO(sdsmith): @perf: find first set optimization

        while (i < s_bit_size) {
            s32 el_idx = i / s_underlying_type_bit_size;
            s32 shift = i % s_underlying_type_bit_size;
            if ((m_data[el_idx] & (1 << shift)) == 0) { return i; }
            ++i;
        }
    }

    void clear() noexcept {
        m_data.fill(0);
    }

    void fill() noexcept {
        m_data.fill(~0U);
    }

    [[nodiscard]] constexpr bool is_set(s32 bit) const noexcept {
        SDS_ASSERT(bit >= 0 && bit < size());
        s32 el_idx = bit / s_underlying_type_bit_size;
        s32 shift = bit % s_underlying_type_bit_size;
        return m_data[el_idx] & (1 << shift);
    }

    constexpr void set(s32 bit) noexcept {
        SDS_ASSERT(bit >= 0 && bit < size());
        s32 el_idx = bit / s_underlying_type_bit_size;
        s32 shift = bit % s_underlying_type_bit_size;
        m_data[el_idx] |= (1 << shift);
    }

    constexpr void unset(s32 bit) noexcept {
        SDS_ASSERT(bit < size());
        s32 el_idx = bit / s_underlying_type_bit_size;
        s32 shift = bit % s_underlying_type_bit_size;
        m_data[el_idx] &= ~(1 << shift);
    }

    /**
     * \brief Number of bits in the bitmask.
     */
    constexpr s32 size() const noexcept {
        return s_bit_size;
    }

private:
    static constexpr s32 s_bit_size = Bit_Size;
    static constexpr s32 s_underlying_type_bit_size = sds::bit_size<underlying_type>();
    std::array<underlying_type, sds::bits_fit_in_num_elements<underlying_type>(s_bit_size)> m_data{};
};
}
