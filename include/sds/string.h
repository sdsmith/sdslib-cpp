#pragma once

namespace sds {
constexpr char const* to_string(bool v) noexcept {
    return (v ? "true" : "false");
}
} // namespace sds
