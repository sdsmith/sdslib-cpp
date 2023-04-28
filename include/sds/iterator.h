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
#    if SDS_CPP_20_SUPPORTED
using contiguous_iterator_tag = std::contiguous_iterator_tag;
#    else
using contiguous_iterator_tag = random_access_iterator_tag;
#    endif
#else
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};
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
    using iterator_category = sds::contiguous_iterator_tag;
    using difference_type = ptrdiff;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
};

/* Specialization for const pointers as iterators. */
template <typename T>
struct Iterator_Traits<T const*> {
    using iterator_category = sds::contiguous_iterator_tag;
    using difference_type = ptrdiff;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
};

template <typename Iterator>
class Reverse_Iterator {
    Iterator m_iterator{};

public:
    using iterator_category = typename sds::Iterator_Traits<Iterator>::iterator_category;
    using difference_type = typename sds::Iterator_Traits<Iterator>::difference_type;
    using value_type = typename sds::Iterator_Traits<Iterator>::value_type;
    using pointer = typename sds::Iterator_Traits<Iterator>::pointer;
    using reference = typename sds::Iterator_Traits<Iterator>::reference;

    Reverse_Iterator() = default;
    explicit Reverse_Iterator(Iterator it) : m_iterator(it) {}

    // TODO(sdsmith): noexcept propagation

    constexpr reference operator*() const
    {
        Iterator it(m_iterator);
        return *--it;
    }

    constexpr pointer operator->() const { return &(operator*()); }

    constexpr Reverse_Iterator& operator++() // prefix
    {
        --m_iterator;
        return *this;
    }

    constexpr Reverse_Iterator operator++(int) // postfix
    {
        Reverse_Iterator it(*this);
        --m_iterator;
        return it;
    }

    constexpr Reverse_Iterator& operator--() // prefix
    {
        ++m_iterator;
        return *this;
    }

    constexpr Reverse_Iterator operator--(int) // postfix
    {
        Reverse_Iterator it(*this);
        ++m_iterator;
        return it;
    }

    constexpr Reverse_Iterator operator+(difference_type n) const
    {
        return Reverse_Iterator(m_iterator - n);
    }

    constexpr Reverse_Iterator& operator+=(difference_type n)
    {
        m_iterator -= n;
        return *this;
    }

    constexpr Reverse_Iterator operator-(difference_type n) const
    {
        return Reverse_Iterator(m_iterator + n);
    }

    constexpr Reverse_Iterator& operator-=(difference_type n)
    {
        m_iterator += n;
        return *this;
    }
};

template <typename Iterator1, typename Iterator2>
constexpr bool operator==(sds::Reverse_Iterator<Iterator1> const& a,
                          sds::Reverse_Iterator<Iterator2> const& b)
{
    return a.m_iterator == b.m_iterator;
}

template <typename Iterator1, typename Iterator2>
constexpr bool operator!=(sds::Reverse_Iterator<Iterator1> const& a,
                          sds::Reverse_Iterator<Iterator2> const& b)
{
    return a.m_iterator != b.m_iterator;
}

} // namespace sds
