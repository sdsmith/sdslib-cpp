#include "gtest/gtest.h"

#include "array/carray.h"

TEST(C_Array, size) {
    char const a[1] = {};
    EXPECT_EQ(sds::carray_size(a), 1U);

    char const b[] = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(sds::carray_size(b), 6U);

    char const c[10] = {};
    EXPECT_EQ(sds::carray_size(c), 10U);
}
