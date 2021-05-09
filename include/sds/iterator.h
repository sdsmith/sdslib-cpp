#pragma once

#include "sds/details/common.h"

#if SDS_USE_STD_ITERATOR_CATEGORIES
#    include <iterator>
#endif

namespace sds
{
#if SDS_USE_STD_ITERATOR_CATEGORIES
using input_iterator_tag = std::input_iterator_tag;
using output_iterator_tag = std::output_iterator_tag;
using forward_iterator_tag = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;
#else
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
#endif

/**
 * Generic access to an iterators traits.
 *
 * NOTE(sdsmith): Don't attempt to access traits directly from an iterator. Some iterators depend on
 * specializations of this type for their traits and don't define them directly.
 */
template <typename Iterator>
struct Iterator_Traits {
    using iterator_category = typename Iterator::iterator_category;
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
};

/* Specialization for pointers as iterators. */
template <typename T>
struct Iterator_Traits<T*> {
    using iterator_category = sds::random_access_iterator_tag;
    using difference_type = ptrdiff;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
};

/* Specialization for const pointers as iterators. */
template <typename T>
struct Iterator_Traits<T const*> {
    using iterator_category = sds::random_access_iterator_tag;
    using difference_type = ptrdiff;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
};

// TODO(sdsmith):
// class Reverse_Iterator {};

} // namespace sds
