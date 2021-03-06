#pragma once

#include <cstddef>

namespace sds
{
    /**
     * \brief Size of c-style array.
     */
    template<typename T, size_t N>
    constexpr size_t carray_size(T (&)[N]) {
        return N;
    }

    /**
     * \brief Byte size of c-style array.
     */
    template<typename T, size_t N>
    constexpr size_t byte_size(T (&)[N]) {
        return N * sizeof(T);
    }

} // namespace sds
