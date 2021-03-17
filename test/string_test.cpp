#include "gtest/gtest.h"

#include "sds/string.h"

TEST(String, byte_size) {
    EXPECT_EQ(sds::byte_size("hello"), 6);
    EXPECT_EQ(sds::byte_size(""), 1);
    EXPECT_EQ(sds::byte_size("hello world"), 12);
}
