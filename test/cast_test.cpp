#include "gtest/gtest.h"

#include "sds/cast.h"
#include "sds/details/types.h"
#include <limits>

using f32 = sds::f32;
using s32 = sds::s32;
using u32 = sds::u32;

TEST(Cast_Test, type_pun)
{
    f32 f = 0.0f;
    EXPECT_EQ(sds::type_pun<s32>(f), 0);
    EXPECT_EQ(sds::type_pun<s32>(&f), 0);

    u32 n = 1U << 31;
    EXPECT_EQ(sds::type_pun<s32>(n), std::numeric_limits<s32>::min());
    EXPECT_EQ(sds::type_pun<s32>(&n), std::numeric_limits<s32>::min());
}
