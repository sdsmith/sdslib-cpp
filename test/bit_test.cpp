#include "gtest/gtest.h"

#include "sds/bit.h"
#include "sds/experimental/const.h"

TEST(BitTest, align_up) {
#define BIT_ALIGN_UP_EQ(Bits, Type, Val) \
    EXPECT_EQ(sds::bit_align_up(Bits, sizeof(Type)), Val);    \
    EXPECT_EQ(sds::bit_align_up<Type>(Bits), Val);

#define BYTE_ALIGN_UP_EQ(Bytes, Type, Val)                       \
    EXPECT_EQ(sds::byte_align_up(Bytes, sizeof(Type)), Val);    \
    EXPECT_EQ(sds::byte_align_up<Type>(Bytes), Val);

    struct Foo { int a; char b; };

    BIT_ALIGN_UP_EQ(1, char, 8);
    BIT_ALIGN_UP_EQ(8, char, 8);
    BIT_ALIGN_UP_EQ(9, char, 16);
    BIT_ALIGN_UP_EQ(29, char, 32);
    BIT_ALIGN_UP_EQ(8, uint32_t, 32);
    BIT_ALIGN_UP_EQ(31, uint32_t, 32);
    BIT_ALIGN_UP_EQ(32, uint32_t, 32);
    BIT_ALIGN_UP_EQ(33, uint32_t, 64);
    BIT_ALIGN_UP_EQ(253, int64_t, 256);
    BIT_ALIGN_UP_EQ(3, Foo, sizeof(Foo) * CHAR_BIT);

    BYTE_ALIGN_UP_EQ(1, char, 1);
    BYTE_ALIGN_UP_EQ(2, char, 2);
    BYTE_ALIGN_UP_EQ(1, int32_t, 4);
    BYTE_ALIGN_UP_EQ(3, int32_t, 4);
    BYTE_ALIGN_UP_EQ(4, int32_t, 4);
    BYTE_ALIGN_UP_EQ(5, int32_t, 8);
    BYTE_ALIGN_UP_EQ(23, uint64_t, 24);
    BYTE_ALIGN_UP_EQ(4, Foo, sizeof(Foo));

#undef BYTE_ALIGN_UP_EQ
#undef BIT_ALIGN_UP_EQ
}

TEST(BitTest, bitmask) {
    EXPECT_EQ(sds::bitmask<32>(), ~0U);
    EXPECT_EQ(sds::bitmask<1>(), 1U);
    EXPECT_EQ(sds::bitmask<2>(), 0b11U);
    EXPECT_EQ(sds::bitmask<4>(), 0xFU);
    EXPECT_EQ(sds::bitmask<9>(), 0x1FFU);
}

TEST(BitTest, bit_size) {
    EXPECT_EQ(sds::bit_size<char>(), 8);
    EXPECT_EQ(sds::bit_size<int32_t>(), 32);
    EXPECT_EQ(sds::bit_size<int64_t>(), 64);
}
