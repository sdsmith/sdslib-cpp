#pragma once

#include "sds/details/common.h"
#include <intrin0.h>
#include <climits>

namespace sds
{
/**
 * \brief Align the number of bits up to the number of bytes per element.
 *
 * \param num_bits Bit count to align.
 * \param element_num_bytes Element size to align to in bytes.
 */
constexpr s32 bit_align_up(s32 num_bits, s32 element_num_bytes) noexcept
{
    // NOTE(sdsmith): For readabilty, would ideally be able to do the following:
    // constexpr s32 bits_per_el = element_num_bytes * CHAR_BIT;
    // constexpr s32 remainder = (num_bits % (element_num_bytes * CHAR_BIT));

    return ((num_bits % (element_num_bytes * CHAR_BIT)) == 0)
               ? num_bits
               : num_bits + (element_num_bytes * CHAR_BIT) -
                     (num_bits % (element_num_bytes * CHAR_BIT));
}

/**
 * \brief Align the number of bits up to the number of bytes of the given type.
 *
 * \tparam T Element type to align to.
 */
template <typename T>
constexpr s32 bit_align_up(s32 num_bits) noexcept
{
    return bit_align_up(num_bits, sizeof(T));
}

/**
 * \brief Align the number of bits up to the number of bytes per element.
 *
 * \param num_bytes Byte count to align.
 * \param element_num_bytes Element size to align to in bytes.
 */
constexpr s32 byte_align_up(s32 num_bytes, s32 element_num_bytes) noexcept
{
    return ((num_bytes % element_num_bytes) == 0)
               ? num_bytes
               : num_bytes + element_num_bytes - (num_bytes % element_num_bytes);

    return bit_align_up(num_bytes * CHAR_BIT, element_num_bytes);
}

/**
 * \brief Align the number of bytes up to the number of bytes of the given type.
 *
 * \tparam T Element type to align to.
 */
template <typename T>
constexpr s32 byte_align_up(s32 num_bytes) noexcept
{
    return byte_align_up(num_bytes, sizeof(T));
}

/**
 * \brief Size of \a T in bits.
 */
template <typename T>
constexpr s32 bit_size() noexcept
{
    return sizeof(T) * CHAR_BIT;
}

/**
 * \brief Number of elements of size \a T that are needed to contain the given number of bits.
 */
template <typename T>
constexpr s32 bits_fit_in_num_elements(s32 num_bits) noexcept
{
    return bit_align_up<T>(num_bits) / (sizeof(T) * CHAR_BIT);
}

/**
 * \brief Number of elements of size \a T that are needed to contain the given number of bytes.
 */
template <typename T>
constexpr s32 bytes_fit_in_num_elements(s32 num_bytes) noexcept
{
    return byte_align_up<T>(num_bytes) / sizeof(T);
}

/**
 * \brief Create a bitmask of size \a N.
 */
template <s32 N>
constexpr u32 bitmask() noexcept
{
    if constexpr (N >= 32) {
        return ~0U;
    } else {
        return (1 << N) - 1;
    }
}

[[nodiscard]] s32 bit_count(u32 x) noexcept
{
#if SDS_USE_COMPILER_BUILTINS
#    if SDS_COMPILER_MSC
    return static_cast<s32>(__popcnt(x));
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
    return __builtin_popcount(x);
#    endif
#else
#    error unimplemented
#endif
}

[[nodiscard]] s32 bit_count(u64 x) noexcept
{
#if SDS_USE_COMPILER_BUILTINS
#    if SDS_COMPILER_MSC
    return static_cast<s32>(__popcnt64(x));
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
    return __builtin_popcountll(x);
#    endif
#else
#    error unimplemented
#endif
}

}; // namespace sds
