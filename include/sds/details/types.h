#pragma once

#include <cstdint>

namespace sds {
    using s8 = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s32f = int_fast32_t; /** At least 32bit, larger if faster */
    using s64 = int64_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u32f = uint_fast32_t; /** At least 32bit, larger if faster */
    using u64 = uint64_t;

    using f32 = float;
    using f64 = double;
}