#include "gtest/gtest.h"

#include "sds/array/make_array.h"

TEST(Make_Array_Test, vargs_constructor) {
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

TEST(Make_Array_Test, string_literal_constructor) {
    // String literal overload

    auto b1 = sds::make_array("hello world");
    EXPECT_EQ(b1.size(), 12);
    EXPECT_EQ(b1.front(), 'h');
    EXPECT_EQ(b1.back(), '\0');

    auto b2 = sds::make_array<char>("hello world");
    EXPECT_EQ(b2.size(), 12);
    EXPECT_EQ(b2.front(), 'h');
    EXPECT_EQ(b2.back(), '\0');

    auto b3 = sds::make_array(L"hello world");
    EXPECT_EQ(b3.size(), 12);
    EXPECT_EQ(b3.front(), 'h');
    EXPECT_EQ(b3.back(), '\0');

    auto b4 = sds::make_array<wchar_t>(L"hello world");
    EXPECT_EQ(b4.size(), 12);
    EXPECT_EQ(b4.front(), 'h');
    EXPECT_EQ(b4.back(), '\0');
}
