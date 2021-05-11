#include "gtest/gtest.h"

#include "sds/bitarray.h"

TEST(BitarrayTest, Bitarray)
{
    sds::Bitarray<3> a;
    EXPECT_EQ(a.size(), 3);
    EXPECT_TRUE(a.none());
    EXPECT_FALSE(a.all());

    a.set(0);
    EXPECT_FALSE(a.none());
    EXPECT_FALSE(a.all());
    EXPECT_TRUE(a.test(0));

    a.reset(0);
    EXPECT_TRUE(a.none());
    EXPECT_FALSE(a.all());
    EXPECT_FALSE(a.test(0));

    a.set(2);
    EXPECT_TRUE(a.test(2));

    a.set_all();
    EXPECT_TRUE(a.all());
    EXPECT_TRUE(a.any());
    EXPECT_FALSE(a.none());

    a.reset_all();
    EXPECT_FALSE(a.all());
    EXPECT_FALSE(a.any());
    EXPECT_TRUE(a.none());
}
