#pragma once

#include <array>
#include <cstddef>

namespace sds
{
/**
 * \brief Return the size of the given array in bytes.
 */
template <typename T, size_t Size>
constexpr size_t byte_size(std::array<T, Size> const& arr)
{
    return Size * sizeof(T);
}
} // namespace sds
