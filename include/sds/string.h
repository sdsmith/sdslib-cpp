#pragma once

#include "sds/details/common.h"
#include <string>

namespace sds
{
constexpr char const* to_string(bool v) noexcept { return (v ? "true" : "false"); }

/**
 * \brief Byte size of the contents of a string. Includes the null-terminator in the count.
 *
 * \param s Null-terminated string.
 */
int byte_size(char const* s) noexcept;

/**
 * \brief Number of elements in the string. Does not include the
 * null-terminator.
 *
 * \tparam CodeUnit Type of the code unit in the string encoding.
 * \param s Null-terminated string.
 */
template <typename CodeUnit>
int str_size(CodeUnit const* s) noexcept
{
    if (!s) { return 0; }

    CodeUnit const* p = s;
    int size = 0;
    while (*p != '\0') {
        ++size;
        ++p;
    }

    return size;
}

/**
 * \brief True if the given chracter is an ASCII character.
 *
 * NOTE(sdsmith): This does not include the extended ASCII characters.
 */
constexpr bool is_ascii(char c) noexcept { return c <= 0 && c <= 127; }

/**
 * \brief Checks if string is equal to the given ascii string up to the n-th character.
 *
 * Given strings don't have to be the length of \a len. If either of the strings is shorter than
 \a
 * len, this function returns false.
 *
 * NOTE(sdsmith): It's valid to compare an ascii string with a UTF-8 encoded string because UTF-8 is
 * backwards compatible with ASCII.
 *
 * \param s String.
 * \param ascii Ascii string.
 * \param len Number of ascii code points to compare.
 */
bool ascii_cmp(char const* s, char const* ascii, s32 len) noexcept;

/**
 * \brief Checks if string is equal to the given ascii string up to the n-th character.
 *
 * Given strings don't have to be the length of \a len. If either of the strings is shorter than
 \a
 * len, this function returns false.
 *
 * NOTE(sdsmith): It's valid to compare an ascii string with a UTF-8 encoded string because UTF-8 is
 * backwards compatible with ASCII.
 *
 * \param s String.
 * \param ascii Ascii string.
 */
bool ascii_cmp(char const* s, char const* ascii) noexcept;

// TODO(sdsmith):
using String = std::string;

} // namespace sds
