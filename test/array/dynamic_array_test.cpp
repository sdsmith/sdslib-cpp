#include "gtest/gtest.h"

#include "sds/array/dynamic_array.h"
#include <iterator>


/** \file dynamic_array_test.cpp
 * \brief \link sds::Dynamic_Array \endlink tests.
 *
 * Tests that the requirements for
 * [Container](https://en.cppreference.com/w/cpp/named_req/Container) and
 * [RandomAccessIterator](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator)
 * are met.
 */

/* TODO(sdsmith):
template <typename T> class Is_Container_Test : public testing::Test {};
using ContainerTypes = testing::Types<sds::Dynamic_Array<int>>;
TYPED_TEST_SUITE(Is_Container_Test, ContainerTypes);

TYPED_TEST(Is_Container_Test, types) {
    // TODO(sdsmith):
    using Container = TypeParam;

    // Container must have the following public types defined, including iterator
    // types (which are covered later).

    // // value_type must be erasable
    // {
    //     // ref: https://en.cppreference.com/w/cpp/named_req/Erasable
    //     ASSERT_EQ(std::is_same_v<C::allocator_type>, std::allocator_traits<C::allocator_type>::rebind_alloc<C::value_type>>);
    //     using A = C::allocator_type;
    //     C c{1};
    //     std::allocator_traits<A>::destroy(c.get_allocator(), c.data());
    // }

    EXPECT_TRUE(std::is_integral_v<typename Container::size_type>);
    EXPECT_TRUE(std::is_integral_v<typename Container::difference_type>);
    EXPECT_TRUE(std::is_reference_v<typename Container::reference>);
    EXPECT_TRUE(std::is_reference_v<typename Container::const_reference> &&
                std::is_const_v<typename Container::const_reference>);

    // Just check that the following exist without restriction on what they are.
    EXPECT_TRUE(!std::is_void_v<typename Container::value_type>);
    EXPECT_TRUE(!std::is_void_v<typename Container::iterator>);
    EXPECT_TRUE(!std::is_void_v<typename Container::const_iterator>);

    // TODO(sdsmith): not strictly necessary, but useful
    //EXPECT_TRUE(std::is_object_v<typename Container::allocator_type>);
    //EXPECT_TRUE(std::is_pointer_v<typename Container::pointer>);
    //EXPECT_TRUE(std::is_pointer_v<typename Container::const_pointer> && std::is_const<typename Container::const_pointer>);
}

TYPED_TEST(Is_Container_Test, container_legacy_forward_iterator) { FAIL(); }

TYPED_TEST(Is_Container_Test, container_const_legacy_forward_iterator) { FAIL(); }

TYPED_TEST(Is_Container_Test, container_constructor) {
    using Container = TypeParam;
    Container c = Container();
    EXPECT_TRUE(c.empty());
}

TYPED_TEST(Is_Container_Test, container_copy_constructor) {
    using Container = TypeParam;
    Container a{1, 2, 3, 4};
    Container b(a);
    EXPECT_EQ(a, b);
}

TYPED_TEST(Is_Container_Test, container_move_constructor) {
    using Container = TypeParam;
    Container a{1, 2, 3};
    Container b(a);
    Container c = a;
    EXPECT_EQ(b, c);
}

TYPED_TEST(Is_Container_Test, container_copy_assignment) {
    using Container = TypeParam;
    Container a{1, 2, 3};
    Container b;
    b = a;
    EXPECT_EQ(a, b);
}

TYPED_TEST(Is_Container_Test, container_move_assignment) {
    using Container = TypeParam;
    Container a{1, 2, 3};
    Container b(a);
    Container c = std::move(a);
    EXPECT_EQ(b, c);
}

TYPED_TEST(Is_Container_Test, container_destructor) {
    // destroys all elements and frees all resources

    // NOTE(sdsmith): can only check if it's callable.
    using Container = TypeParam;
    Container a = {1, 2, 3};
    a.~Container();
}

TYPED_TEST(Is_Container_Test, container_iterators) {
    using Container = TypeParam;
    Container a{1, 2, 3};
    // Check they are callable;
    EXPECT_NE(a.begin(), a.end());
    EXPECT_NE(a.cbegin(), a.cend());
}

TYPED_TEST(Is_Container_Test, container_equality) {
    using Container = TypeParam;
    Container a{1, 2, 3};
    Container b{1, 2, 3};
    Container c{1, 2, 3, 4};
    Container d{1, 3, 2};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_EQ(a.size(), b.size());
    EXPECT_TRUE(std::equal(a.begin(), a.end(), b.begin(), b.end()));

    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_NE(a.size(), c.size());
    EXPECT_FALSE(std::equal(a.begin(), a.end(), c.begin(), c.end()));

    EXPECT_FALSE(a == d);
    EXPECT_TRUE(a != d);
    EXPECT_NE(a.size(), d.size());
    EXPECT_FALSE(std::equal(a.begin(), a.end(), d.begin(), d.end()));
}

TYPED_TEST(Is_Container_Test, container_swap) {
    // a.swap(b)
    // swap(a, b)
    FAIL();
}

TYPED_TEST(Is_Container_Test, container_size) {
    // std::distance(a.begin(), a.end())
    using Container = TypeParam;
    Container a;
    Container b{1};
    Container c{1, 2};
    Container d{1, 2, 3};
    EXPECT_EQ(a.size(), std::distance(a.begin(), a.end()));
    EXPECT_EQ(b.size(), std::distance(b.begin(), b.end()));
    EXPECT_EQ(c.size(), std::distance(c.begin(), c.end()));
    EXPECT_EQ(d.size(), std::distance(d.begin(), d.end()));
}

TYPED_TEST(Is_Container_Test, container_max_size) {
    using Container = TypeParam;
    // b.size() where b is the largest possible container

    // NOTE(sdsmith): Can only really check its existance and that it's at least
    // as big as size.

    Container a;
    EXPECT_GE(a.max_size(), 0);
    EXPECT_GE(a.max_size(), a.size());

    Container b{1};
    EXPECT_GE(b.max_size(), 0);
    EXPECT_GE(b.max_size(), a.size());

    Container c{1, 2, 3, 4};
    EXPECT_GE(c.max_size(), 0);
    EXPECT_GE(c.max_size(), a.size());
}

TYPED_TEST(Is_Container_Test, container_empty) {
    using Container = TypeParam;

    Container a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.begin(), a.end());

    Container b{1};
    EXPECT_FALSE(a.empty());
    EXPECT_NE(a.begin(), a.end());
}

TYPED_TEST(Is_Container_Test, iterator_types) {
    using Container = TypeParam;

    // difference_type must be the same as
    EXPECT_TRUE((std::is_base_of_v<std::input_iterator_tag,
                 typename std::iterator_traits<typename Container::iterator>::iterator_category>));

    EXPECT_TRUE(std::is_integral_v<std::iterator_traits<typename Container::iterator>::difference_type>);
    EXPECT_TRUE((std::is_same_v<typename Container::value_type, std::iterator_traits<typename Container::iterator>::value_type>));
    std::iterator_traits<typename Container::iterator>::pointer;
    std::iterator_traits<typename Container::iterator>::reference;

}

TYPED_TEST(Is_Container_Test, const_iterator_types) {
    using Container = TypeParam;
    std::iterator_traits<typename Container::const_iterator>::iterator_category;
    std::iterator_traits<typename Container::const_iterator>::difference_type;
    std::iterator_traits<typename Container::const_iterator>::value_type;
    std::iterator_traits<typename Container::const_iterator>::pointer;
    std::iterator_traits<typename Container::const_iterator>::reference;
}
*/
