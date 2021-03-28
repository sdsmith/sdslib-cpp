#pragma once

#include "sds/details/common.h"

namespace sds {
constexpr char const* to_string(bool v) noexcept {
    return (v ? "true" : "false");
}

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
template<typename CodeUnit>
int str_size(CodeUnit const* s) noexcept {
    if (!s) { return 0; }

    CodeUnit const* p = s;
    int size = 0;
    while (*p != '\0') {
        ++size;
        ++p;
    }

    return size;
}

} // namespace sds
