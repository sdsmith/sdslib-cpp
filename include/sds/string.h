#pragma once

#include "sds/details/common.h"

namespace sds {
constexpr char const* to_string(bool v) noexcept {
    return (v ? "true" : "false");
}

/**
 * Byte size of the contents of a string. Includes the null-terminator in the
 * count.
 *
 * \param s Null-terminated string.
 */
int byte_size(char const* s) noexcept;

} // namespace sds
