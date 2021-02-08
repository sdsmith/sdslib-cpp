#include "gtest/gtest.h"

#include "list/linked_list.h"

TEST(Linked_List_Test, constructor) {
    // TODO(sdsmith):
    sds::Linked_List<int> l1;
    EXPECT_EQ(l1.size(), 0U);
    EXPECT_TRUE(l1.empty());

    sds::Linked_List<int> l2{};
    EXPECT_EQ(l2.size(), 0U);
    EXPECT_TRUE(l2.empty());

    sds::Linked_List<int> l3{1};
    EXPECT_EQ(l3.size(), 1U);
    EXPECT_FALSE(l3.empty());
    EXPECT_EQ(l3.front(), 1);
    EXPECT_EQ(l3.back(), 1);

    sds::Linked_List<int> l4{1, 2};
    EXPECT_EQ(l4.size(), 2U);
    EXPECT_FALSE(l4.empty());
    EXPECT_EQ(l4.front(), 1);
    EXPECT_EQ(l4.back(), 2);

    sds::Linked_List<int> l5{1, 2, 3, 4, 5};
    EXPECT_EQ(l5.size(), 5U);
    EXPECT_FALSE(l5.empty());
    EXPECT_EQ(l5.front(), 1);
    EXPECT_EQ(l5.back(), 5);
}

TEST(Linked_List_Test, copy_constructed) {
    {
        sds::Linked_List<int> l1;
        sds::Linked_List<int> l2(l1);
        EXPECT_TRUE(l2.empty());
    }

    {
        sds::Linked_List<int> l1{1};
        sds::Linked_List<int> l2(l1);
        EXPECT_FALSE(l2.empty());
        EXPECT_EQ(l2.size(), 2U);
        EXPECT_EQ(l2.front(), 1);
        EXPECT_EQ(l2.back(), 1);
    }

    {
        sds::Linked_List<int> l1{1, 2, 3, 4, 5};
        sds::Linked_List<int> l2(l1);
        ASSERT_EQ(l1.size(), 5);
        EXPECT_EQ(l1.size(), l2.size());
        ASSERT_FALSE(l1.empty());
        EXPECT_EQ(l1.empty(), l2.empty());
        EXPECT_EQ(l2.front(), 1);
        EXPECT_EQ(l2.back(), 5);
    }

}

struct Foo {
    int n = 0;
    Foo(int n) : n(n) {}
    Foo(Foo&& o) : n(std::move(o.n)) {}
    Foo(Foo const& o) : n(o.n) {}
};

TEST(Linked_List_Test, move_constructed) {
    // TODO(sdsmith):
    sds::Linked_List<Foo> l1{Foo(0), Foo(1), Foo(2)};
    sds::Linked_List<Foo> l2(l1);
}

TEST(Linked_List_Test, modify_front) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, modify_back) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, remove) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, clear) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, empty) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, size) {
    // TODO(sdsmith):
}

TEST(Linked_List_Test, non_pod_element) {

}
