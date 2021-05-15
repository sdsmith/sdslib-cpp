#include "sds/details/common.h"

namespace sds
{
/**
 * \brief Convert temporary (and other stuff) to an lvalue.
 *
 * Simulates the C language concept of compound literal, albeit with a shorter lifetime.
 */
template <typename T>
T& lvalue_cast(T&& t)
{
    return t;
}
} // namespace sds
