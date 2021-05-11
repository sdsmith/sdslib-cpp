#include "gtest/gtest.h"

#include "sds/array.h"

using namespace sds;

TEST(Array_Test, constructor) { Array<int, 10> arr; }
