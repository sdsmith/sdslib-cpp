#include "sds/bit.h"

using namespace sds;

s32 bit_count(u32 x) noexcept
{
#if SDS_USE_COMPILER_BUILTINS
#    if SDS_COMPILER_MSC
    return static_cast<s32>(__popcnt(x));
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
    return __builtin_popcount(x);
#    endif
#else
#    error unimplemented
#endif
}

s32 bit_count(u64 x) noexcept
{
#if SDS_USE_COMPILER_BUILTINS
#    if SDS_COMPILER_MSC
    return static_cast<s32>(__popcnt64(x));
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
    return __builtin_popcountll(x);
#    endif
#else
#    error unimplemented
#endif
}
