#include "gtest/gtest.h"

#include "sdslib/array/make_array.h"

TEST(Make_Array_Test, constructor) {
    // NOTE(sdsmith): Not valid to create an empty array
    // auto a = sds::make_array<int>();

    auto a1 = sds::make_array<int>(1, 2, 3);
    EXPECT_EQ(a1.size(), 3U);
    EXPECT_EQ(a1.front(), 1);
    EXPECT_EQ(a1.back(), 3);

    auto a2 = sds::make_array(1, 2, 3);
    EXPECT_EQ(a2.size(), 3U);
    EXPECT_EQ(a2.front(), 1);
    EXPECT_EQ(a2.back(), 3);
}
