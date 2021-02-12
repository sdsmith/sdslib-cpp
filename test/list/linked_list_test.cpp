#include "gtest/gtest.h"

#include "list/linked_list.h"
#include "carray.h"
#include <vector>


TEST(Linked_List_Test, constructor) {
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
        EXPECT_EQ(l2.size(), 1U);
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

TEST(Linked_List_Test, copy_assignment) {
    sds::Linked_List<int> l1{1};
    sds::Linked_List<int> l2 = l1;
    EXPECT_FALSE(l2.empty());
    EXPECT_EQ(l2.size(), 1U);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 1);

    sds::Linked_List<int> l3{1, 3, 4, 5, 6};
    l2 = l3;
    EXPECT_FALSE(l2.empty());
    EXPECT_EQ(l2.size(), 5U);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 6);
    {
        auto it2 = l2.begin();
        auto it3 = l3.begin();
        for (int i = 0; it2 != l2.end(); ++it2, ++it3, ++i) {
            EXPECT_EQ(*it2, *it3) << "on loop " << i;
        }
    }
}

TEST(Linked_List_Test, move_constructed) {
    {
        sds::Linked_List<int> l1;
        sds::Linked_List<int> l2(std::move(l1));
        EXPECT_EQ(l2.size(), 0U);
        EXPECT_TRUE(l2.empty());
    }

    {
        sds::Linked_List<int> l1{1};
        sds::Linked_List<int> l2(std::move(l1));
        EXPECT_EQ(l2.size(), 1U);
        EXPECT_FALSE(l2.empty());
        EXPECT_EQ(l2.front(), 1);
        EXPECT_EQ(l2.back(), 1);
    }

    {
        sds::Linked_List<int> l1{0, 1, 2};
        sds::Linked_List<int> l2(std::move(l1));
        EXPECT_EQ(l2.size(), 3U);
        EXPECT_FALSE(l2.empty());
        EXPECT_EQ(l2.front(), 0);
        EXPECT_EQ(l2.back(), 2);
    }
}

TEST(Linked_List_Test, move_assignment) {
    sds::Linked_List<int> l1{1};
    sds::Linked_List<int> l2 = std::move(l1);
    EXPECT_FALSE(l2.empty());
    EXPECT_EQ(l2.size(), 1U);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 1);

    sds::Linked_List<int> l3{1, 3, 4, 5, 6};
    l1 = l3;
    l2 = std::move(l3);
    EXPECT_FALSE(l2.empty());
    EXPECT_EQ(l2.size(), 5U);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 6);
    {
        auto it2 = l2.begin();
        auto it1 = l1.begin();
        for (int i = 0; it2 != l2.end(); ++it2, ++it1, ++i) {
            EXPECT_EQ(*it2, *it1) << "on loop " << i;
        }
    }
}

TEST(Linked_List_Test, clear) {
    sds::Linked_List<int> l;
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0U);

    l = sds::Linked_List<int>{1};
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0U);

    l = sds::Linked_List<int>{9, 8, 17, 3, 5};
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0U);
}

TEST(Linked_List_Test, empty) {
    sds::Linked_List<int> l1;
    EXPECT_TRUE(l1.empty());
    sds::Linked_List<int> l2{1};
    EXPECT_FALSE(l2.empty());
    sds::Linked_List<int> l3{1, 2, 3};
    EXPECT_FALSE(l3.empty());

    l2.pop_back();
    EXPECT_TRUE(l2.empty());
    l3.pop_back();
    EXPECT_FALSE(l3.empty());
    l3.pop_back();
    EXPECT_FALSE(l3.empty());
    l3.pop_back();
    EXPECT_TRUE(l3.empty());
}

TEST(Linked_List_Test, size) {
    sds::Linked_List<int> l1;
    EXPECT_EQ(l1.size(), 0U);
    sds::Linked_List<int> l2{1};
    EXPECT_EQ(l2.size(), 1U);
    sds::Linked_List<int> l3{1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(l3.size(), 9U);
}

TEST(Linked_List_Test, push_front) {
    sds::Linked_List<int> l;
    l.push_front(1);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1U);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 1);

    l.push_front(2);
    EXPECT_EQ(l.size(), 2U);
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(l.back(), 1);

    l.push_front(3);
    EXPECT_EQ(l.size(), 3U);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 1);
}


TEST(Linked_List_Test, push_back) {
    sds::Linked_List<int> l;
    l.push_back(1);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1U);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 1);

    l.push_back(2);
    EXPECT_EQ(l.size(), 2U);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 2);

    l.push_back(3);
    EXPECT_EQ(l.size(), 3U);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST(Linked_List_Test, pop_front) {
    {
        sds::Linked_List<int> l{1};
        l.pop_front();
        EXPECT_EQ(l.size(), 0U);
        EXPECT_TRUE(l.empty());
    }

    {
        sds::Linked_List<int> l{1, 2, 3};

        l.pop_front();
        EXPECT_EQ(l.size(), 2U);
        EXPECT_EQ(l.front(), 2);
        EXPECT_EQ(l.back(), 3);

        l.pop_front();
        EXPECT_EQ(l.size(), 1U);
        EXPECT_EQ(l.front(), 3);
        EXPECT_EQ(l.back(), 3);

        l.pop_front();
        EXPECT_EQ(l.size(), 0U);
        EXPECT_TRUE(l.empty());
    }
}

TEST(Linked_List_Test, pop_back) {
    {
        sds::Linked_List<int> l{1};
        l.pop_back();
        EXPECT_EQ(l.size(), 0U);
        EXPECT_TRUE(l.empty());
    }

    {
        sds::Linked_List<int> l{1, 2, 3};

        l.pop_back();
        EXPECT_EQ(l.size(), 2U);
        EXPECT_EQ(l.front(), 1);
        EXPECT_EQ(l.back(), 2);

        l.pop_back();
        EXPECT_EQ(l.size(), 1U);
        EXPECT_EQ(l.front(), 1);
        EXPECT_EQ(l.back(), 1);

        l.pop_back();
        EXPECT_EQ(l.size(), 0U);
        EXPECT_TRUE(l.empty());
    }
}

TEST(Linked_List_Test, modify_front) {
    sds::Linked_List<int> l;
    l.push_front(1);
    l.pop_front();
    EXPECT_TRUE(l.empty());
    l.push_front(10);
    l.push_front(11);
    l.pop_front();
    EXPECT_EQ(l.size(), 1U);
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 10);
    l.push_front(12);
    l.push_front(13);
    l.pop_front();
    EXPECT_EQ(l.size(), 2U);
    EXPECT_EQ(l.front(), 12);
    EXPECT_EQ(l.back(), 10);
    l.pop_front();
    EXPECT_EQ(l.size(), 1U);
    l.pop_front();
    EXPECT_TRUE(l.empty());
}

TEST(Linked_List_Test, modify_back) {
    sds::Linked_List<int> l;
    l.push_back(1);
    l.pop_back();
    EXPECT_TRUE(l.empty());
    l.push_back(10);
    l.push_back(11);
    l.pop_back();
    EXPECT_EQ(l.size(), 1U);
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 10);
    l.push_back(12);
    l.push_back(13);
    l.pop_back();
    EXPECT_EQ(l.size(), 2U);
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 12);
    l.pop_back();
    EXPECT_EQ(l.size(), 1U);
    l.pop_back();
    EXPECT_TRUE(l.empty());
}

TEST(Linked_List_Test, modify) {
    sds::Linked_List<int> l;
    l.push_back(1);
    l.pop_front();
    EXPECT_TRUE(l.empty());
    l.push_front(2);
    l.pop_back();
    EXPECT_TRUE(l.empty());

    l.push_back(1);
    l.push_front(2);
    l.push_back(3);
    l.push_front(4);
    l.push_front(5);
    l.push_back(6);
    constexpr int arr[] = {5, 4, 2, 1, 3, 6};
    auto it = l.cbegin();
    for (int i = 0; static_cast<size_t>(i) < sds::carray_size(arr); ++i, ++it) {
        EXPECT_EQ(*it, arr[i]) << "on loop " << i;
    }

    l.pop_back();
    l.pop_back();
    l.pop_front();
    l.push_front(10);
    l.pop_back();
    l.pop_back();
    l.pop_front();
    l.pop_front();
    EXPECT_TRUE(l.empty());
}

TEST(Linked_List_Test, remove) {
    sds::Linked_List<int> l{10, 3, 16, 4, 7, 8, 4, 1};

    // Remove front
    l.remove(10);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 1);
    EXPECT_EQ(l.size(), 7U);
    EXPECT_FALSE(l.empty());

    // Remove back
    l.remove(1);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 4);
    EXPECT_EQ(l.size(), 6U);
    EXPECT_FALSE(l.empty());

    // Remove from middle
    l.remove(7);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 4);
    EXPECT_EQ(l.size(), 5U);
    EXPECT_FALSE(l.empty());
    {
        constexpr int arr[] = {3, 16, 4, 8, 4};
        auto it = l.cbegin();
        for (int i = 0; static_cast<size_t>(i) < sds::carray_size(arr); ++i, ++it) {
            EXPECT_EQ(*it, arr[i]) << "on loop " << i;
        }
    }

    // Remove duplicate (make sure it picks the first duplicate)
    l.remove(4);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 4);
    EXPECT_EQ(l.size(), 4U);
    EXPECT_FALSE(l.empty());
    {
        constexpr int arr[] = {3, 16, 8, 4};
        auto it = l.cbegin();
        for (int i = 0; static_cast<size_t>(i) < sds::carray_size(arr); ++i, ++it) {
            EXPECT_EQ(*it, arr[i]) << "on loop " << i;
        }
    }

    // Remove second of duplicate
    l.remove(4);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 8);
    EXPECT_EQ(l.size(), 3U);
    EXPECT_FALSE(l.empty());
    {
        constexpr int arr[] = {3, 16, 8};
        auto it = l.cbegin();
        for (int i = 0; static_cast<size_t>(i) < sds::carray_size(arr); ++i, ++it) {
            EXPECT_EQ(*it, arr[i]) << "on loop " << i;
        }
    }

    // Add something
    l.push_front(8);

    // Remove
    l.remove(16);
    EXPECT_EQ(l.front(), 8);
    EXPECT_EQ(l.back(), 8);
    EXPECT_EQ(l.size(), 3U);
    EXPECT_FALSE(l.empty());
    {
        constexpr int arr[] = {8, 3, 8};
        auto it = l.cbegin();
        for (int i = 0; static_cast<size_t>(i) < sds::carray_size(arr); ++i, ++it) {
            EXPECT_EQ(*it, arr[i]) << "on loop " << i;
        }
    }

    // Remove uncontained element
    l.remove(100);
    EXPECT_EQ(l.front(), 8);
    EXPECT_EQ(l.back(), 8);
    EXPECT_EQ(l.size(), 3U);
    EXPECT_FALSE(l.empty());

    // Remove array element, creating an empty array
    l.remove(8);
    l.remove(3);
    EXPECT_EQ(l.front(), 8);
    EXPECT_EQ(l.back(), 8);
    EXPECT_EQ(l.size(), 1U);
    EXPECT_FALSE(l.empty());
    l.remove(8);
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0U);
}

TEST(Linked_List_Test, contains) {
    sds::Linked_List<int> l;
    EXPECT_FALSE(l.contains(0));
    l.push_back(1);
    EXPECT_TRUE(l.contains(1));
    l.push_back(2);
    EXPECT_TRUE(l.contains(1));
    EXPECT_TRUE(l.contains(2));
    l.push_front(2);
    EXPECT_TRUE(l.contains(1));
    EXPECT_TRUE(l.contains(2));
    l.pop_front();
    EXPECT_TRUE(l.contains(1));
    EXPECT_TRUE(l.contains(2));
    l.pop_back();
    EXPECT_TRUE(l.contains(1));
    EXPECT_FALSE(l.contains(2));
    l.pop_back();
    EXPECT_FALSE(l.contains(1));
    EXPECT_FALSE(l.contains(2));
}

TEST(Linked_List_Test, iterator) {
    sds::Linked_List<int> l1{0, 1, 2, 3, 4, 5};
    EXPECT_EQ(*l1.begin(), 0);
    EXPECT_EQ(std::distance(l1.begin(), l1.end()), 6);
    // Ensure that the STL range functions work
    EXPECT_EQ(std::distance(std::begin(l1), std::end(l1)), 6);

    {
        auto it = l1.begin();
        for (int n = 0;
             static_cast<size_t>(n) < l1.size();
             n++, it++)
        {
            EXPECT_EQ(*it, n);
        }
        ASSERT_EQ(it, l1.end());
    }

    { // pre and post inc the same
        auto it1 = l1.begin();
        auto it2 = l1.begin();
        for (int i = 0; it1 != l1.end(); ++it1, it2++, ++i) {
            EXPECT_EQ(*it1, *it2) << "on loop " << i;
        }
    }
}

TEST(Linked_List_Test, const_iterator) {
    sds::Linked_List<int> l1{0, 1, 2, 3, 4, 5};
    EXPECT_EQ(*l1.cbegin(), 0);
    EXPECT_EQ(std::distance(l1.cbegin(), l1.cend()), 6);
    // Ensure that the STL range functions work
    EXPECT_EQ(std::distance(std::cbegin(l1), std::cend(l1)), 6);

    {
        auto it = l1.cbegin();
        for (int n = 0;
             static_cast<size_t>(n) < l1.size();
             n++, it++)
        {
            EXPECT_EQ(*it, n);
        }
        ASSERT_EQ(it, l1.cend());
    }

    { // pre and post inc the same
        auto it1 = l1.cbegin();
        auto it2 = l1.cbegin();

        for (int i = 0; it1 != l1.cend(); ++it1, it2++, ++i) {
            EXPECT_EQ(*it1, *it2) << "on loop " << i;
        }
    }
}

TEST(Linked_List_Test, modify_through_iterator) {
    sds::Linked_List<int> l1{1, 2, 3, 4, 5};

    for (auto it = l1.begin(); it != l1.end(); it++) {
        *it += 1;
    }

    int n = 2;
    for (auto it = l1.begin();
         it != l1.end();
         it++, n++)
    {
        EXPECT_EQ(*it, n);
    }
}

struct Foo {
    int n = 0;
    Foo(int n) : n(n) {}
    Foo(Foo const& o) : n(o.n) {}
    friend bool operator==(Foo const& a, Foo const& b) {
        return a.n == b.n;
    }
    friend std::ostream& operator<<(std::ostream& os, Foo const& a) {
        return os << "Foo(" << a.n << ")";
    }
};

TEST(Linked_List_Test, non_pod_element) {
    sds::Linked_List<Foo> l1{Foo(0), Foo(1), Foo(2)};
    sds::Linked_List<Foo> l2(std::move(l1));
    EXPECT_EQ(l2.front(), Foo(0));
    EXPECT_EQ(l2.back(), Foo(2));
    EXPECT_EQ(l2.size(), 3U);
    EXPECT_FALSE(l2.empty());

    l2.push_back(Foo(3));
    EXPECT_EQ(l2.front(), Foo(0));
    EXPECT_EQ(l2.back(), Foo(3));
    EXPECT_EQ(l2.size(), 4U);
    EXPECT_FALSE(l2.empty());

    l1 = l2;
    auto it1 = l1.cbegin();
    auto it2 = l2.cbegin();
    for (int i = 0; it1 != l1.cend(); ++it1, ++it2, ++i) {
        EXPECT_EQ(*it1, *it2) << "on loop " << i;
    }

}
