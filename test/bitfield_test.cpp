#include "gtest/gtest.h"

#include "sds/bitfield.h"

TEST(BitfieldTest, bitfield) {
    sds::Bitfield<3> mask;
    EXPECT_EQ(mask.size(), 3);
    EXPECT_TRUE(mask.empty());
    EXPECT_FALSE(mask.full());
    EXPECT_EQ(mask.first_unset(), 0);

    mask.set(0);
    EXPECT_EQ(mask.first_unset(), 1);
    EXPECT_FALSE(mask.empty());
    EXPECT_FALSE(mask.full());
    EXPECT_TRUE(mask.is_set(0));

    mask.unset(0);
    EXPECT_TRUE(mask.empty());
    EXPECT_FALSE(mask.full());
    EXPECT_EQ(mask.first_unset(), 0);
    EXPECT_FALSE(mask.is_set(0));

    mask.set(2);
    EXPECT_EQ(mask.first_unset(), 0);
    EXPECT_TRUE(mask.is_set(2));

    mask.fill();
    EXPECT_TRUE(mask.full());
    EXPECT_FALSE(mask.empty());

    mask.clear();
    EXPECT_FALSE(mask.full());
    EXPECT_TRUE(mask.empty());
}
