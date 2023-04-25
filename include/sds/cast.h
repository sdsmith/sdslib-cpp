#include "sds/details/common.h"

#include <array>
#include <cstring>
#include <type_traits>

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

/**
 * \brief Perform type punning without UB.
 *
 * Usage: `float a = 0; int b = type_pun<int>(a);`
 * ref: https://www.youtube.com/watch?v=sCjZuvtJd-k
 */
template <typename ToT, typename FromT>
ToT type_pun(FromT const& a) noexcept
{
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<FromT>);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<ToT>);
    SDS_STATIC_ASSERT(std::is_trivially_constructible_v<ToT>);
    SDS_STATIC_ASSERT_MSG(sizeof(ToT) >= sizeof(std::remove_pointer_t<FromT>),
                          "destination type too small to hold source type");

    ToT ret{};
    if constexpr (std::is_pointer_v<FromT> || std::is_array_v<FromT>) {
        std::memcpy(&ret, a, sizeof(ret));
    } else {
        std::memcpy(&ret, &a, sizeof(ret));
    }
    return ret;
}
// TODO(sdsmith): convert pointers with type_pun and through pointers with type_pun_p

/**
 * \brief Perform type punning without UB from an array.
 *
 * Usage: `float a = 0; int b = type_pun<int>(a);`
 * ref: https://www.youtube.com/watch?v=sCjZuvtJd-k
 */
template <typename ToT, typename FromT>
ToT type_pun(FromT const* a) noexcept
{
    SDS_ASSERT(a);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<FromT>);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<ToT>);
    SDS_STATIC_ASSERT(std::is_trivially_constructible_v<ToT>);
    SDS_STATIC_ASSERT_MSG(sizeof(ToT) >= sizeof(std::remove_pointer_t<FromT>),
                          "destination type too small to hold source type");
}

/**
 * \brief Perform type punning without UB from an array.
 *
 * Usage: `float a = 0; int b = type_pun<int>(a);`
 * ref: https://www.youtube.com/watch?v=sCjZuvtJd-k
 */
template <typename ToT, typename FromT, size_t N>
ToT type_pun(std::array<FromT, N> const* a) noexcept
{
    // TODO(sdsmith):
    SDS_ASSERT(a);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<FromT>);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<ToT>);
    SDS_STATIC_ASSERT(std::is_trivially_constructible_v<ToT>);
    SDS_STATIC_ASSERT_MSG(sizeof(ToT) >= sizeof(std::remove_pointer_t<FromT>) * N,
                          "destination type too small to hold source type");

    ToT ret{};
    std::memcpy(&ret, a, sizeof(ret));
    return ret;
}

/**
 * \brief Perform type punning without UB from an array.
 *
 * ref: https://www.youtube.com/watch?v=sCjZuvtJd-k
 */
template <typename ToT, typename FromT>
void type_pun(ToT* b, FromT const* a) noexcept
{
    SDS_ASSERT(b);
    SDS_ASSERT(a);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<FromT>);
    SDS_STATIC_ASSERT(std::is_trivially_copyable_v<ToT>);
    SDS_STATIC_ASSERT(std::is_trivially_constructible_v<ToT>);

    std::memcpy(b, a, sizeof(ToT));
}
} // namespace sds
