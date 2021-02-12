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
} // namespace sds
