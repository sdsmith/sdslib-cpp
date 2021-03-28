#pragma once

#include <climits>

namespace sds {
/**
 * \brief Align the number of bits up to the number of bytes per element.
 *
 * \param num_bits Bit count to align.
 * \param element_num_bytes Element size to align to in bytes.
 */
constexpr int bit_align_up(int num_bits, int element_num_bytes) noexcept {
    // NOTE(sdsmith): For readabilty, would ideally be able to do the following:
    // constexpr int bits_per_el = element_num_bytes * CHAR_BIT;
    // constexpr int remainder = (num_bits % (element_num_bytes * CHAR_BIT));

    return ((num_bits % (element_num_bytes * CHAR_BIT)) == 0)
        ? num_bits
        : num_bits + (element_num_bytes * CHAR_BIT) - (num_bits % (element_num_bytes * CHAR_BIT));
}

/**
 * \brief Align the number of bits up to the number of bytes of the given type.
 *
 * \tparam T Element type to align to.
 */
template<typename T>
constexpr int bit_align_up(int num_bits) noexcept {
    return bit_align_up(num_bits, sizeof(T));
}

/**
 * \brief Align the number of bits up to the number of bytes per element.
 *
 * \param num_bytes Byte count to align.
 * \param element_num_bytes Element size to align to in bytes.
 */
constexpr int byte_align_up(int num_bytes, int element_num_bytes) noexcept {
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
template<typename T>
constexpr int byte_align_up(int num_bytes) noexcept {
    return byte_align_up(num_bytes, sizeof(T));
}

/**
 * \brief Size of \a T in bits.
 */
template<typename T>
constexpr int bit_size() {
    return sizeof(T) * CHAR_BIT;
}

};
