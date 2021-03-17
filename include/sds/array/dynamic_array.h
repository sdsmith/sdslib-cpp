#pragma once

#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>


namespace sds
{

/*
  TODO(sdsmith): https://en.cppreference.com/w/cpp/named_req/ContiguousIterator
  - check reqs in unittest
  https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
  - check in unittests
*/


template<typename T, typename Allocator = std::allocator<T>>
class Dynamic_Array {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    template<typename ValueT>
    class Iterator
    {
    public:
        // @c++20
        //using iterator_category = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = ValueT;
        using pointer = value_type*;
        using reference = value_type&;

        constexpr Iterator(pointer p) : m_p(p) {}
        constexpr const_reference operator*() const { return *m_p; }
        constexpr pointer operator->() const { return m_p; }
        constexpr Iterator& operator++() { m_p++; return *this; }
        constexpr Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }
        constexpr Iterator operator--() { m_p--; return *this; }
        constexpr Iterator operator--(int) { Iterator it = *this; --(*this); return it; }

        constexpr Iterator& operator+=(difference_type n) { m_p += n; return *this; }
        constexpr friend Iterator operator+(Iterator const& a, difference_type n) { return Iterator(a.m_p + n); }
        constexpr friend Iterator operator+(difference_type n, Iterator const& a) { return a + n; }

        constexpr Iterator& operator-=(difference_type n) { m_p -= n; return *this; }
        constexpr friend Iterator operator-(Iterator const& a, difference_type n) { return Iterator(a.m_p + n); }
        constexpr friend Iterator operator-(difference_type n, Iterator const& b) { return b - n; }
        constexpr friend difference_type operator-(Iterator const& a, Iterator const& b) { return b.m_p - a.m_p; }
        constexpr reference operator[](difference_type n) const { return *(*this + n); }

        // TODO(sdsmith): comparing pointers is _yikes_
        constexpr friend bool operator<(Iterator const& a, Iterator const& b) { return a.m_p < b.m_p; }
        constexpr friend bool operator>=(Iterator const& a, Iterator const& b) { return a.m_p > b.m_p; }
        constexpr friend bool operator<=(Iterator const& a, Iterator const& b) { return a.m_p <= b.m_p; }
        constexpr friend bool operator>(Iterator const& a, Iterator const& b) { return a.m_p >= b.m_p; }
        constexpr friend bool operator==(Iterator const& a, Iterator const& b) { return a.m_p == b.m_p; }
        constexpr friend bool operator!=(Iterator const& a, Iterator const& b) { return !(a == b); }

    private:
        pointer m_p = nullptr;
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Dynamic_Array() {}
    Dynamic_Array(std::initializer_list<T> l);
    Dynamic_Array(Dynamic_Array const& o);
    Dynamic_Array(Dynamic_Array&& o);
    ~Dynamic_Array();

    Dynamic_Array& operator=(Dynamic_Array const& o) noexcept(false); /*{
        clear();

        if (capacity() < o.size()) {
            reallocate(o.size());
        }
        std::copy(o.begin(), o.end(), begin());
        }*/

    Dynamic_Array& operator=(Dynamic_Array&& o) {
        deallocate();

        m_data = std::move(o.m_data);
        m_size = o.m_size;
        m_capacity = o.m_capacity;
    }

    iterator begin() const { return iterator(m_data); }
    iterator end() const { return iterator(m_data + m_size); }
    const_iterator cbegin() const { return const_iterator(m_data); }
    const_iterator cend() const { return const_iterator(m_data + m_size); }

    reference front() {
        SDS_ASSERT(!empty());
        return *m_data;
    }

    const_reference front() const {
        SDS_ASSERT(!empty());
        return *m_data;
    }

    reference back() {
        SDS_ASSERT(!empty());
        return m_data[m_size - 1];
    }

    const_reference back() const {
        SDS_ASSERT(!empty());
        return m_data[m_size - 1];
    }

    reference data() { return m_data; }
    const_reference data() const { return m_data; }

    constexpr void push_back(T const& v);
    constexpr void push_back(T&& v);
    template<typename... Args>
    constexpr reference emplace_back(Args&&... args);

    constexpr iterator insert(const_iterator pos, T const& value);
    constexpr iterator insert(const_iterator pos, T&& value);
    template<typename InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
    constexpr iterator insert(const_iterator pos, std::initializer_list<T> l);

    constexpr void clear();
    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator first, const_iterator last);

    constexpr reference at(size_type index) noexcept(false) {
        range_check(index);
        return (*this)[index];
    }
    constexpr const_reference at(size_type index) const noexcept(false) {
        range_check(index);
        return (*this)[index];
    }

    constexpr reference operator[](size_type index) noexcept {
        SDS_ASSERT(!empty());
        return m_data[index];
    }
    constexpr const_reference operator[](size_type index) const noexcept {
        SDS_ASSERT(!empty());
        return m_data[index];
    }

    constexpr bool empty() const noexcept { return size() != 0; }
    constexpr size_type size() const noexcept { return m_size; }
    constexpr size_type max_size() const noexcept { return m_max_size; }
    constexpr size_type capacity() const noexcept { return m_capacity; }

    constexpr void reserve(size_t);
    // TODO(sdsmith): shrink_to_fit

    constexpr void swap(Dynamic_Array& o);

    friend bool operator==(Dynamic_Array<T> const& a, Dynamic_Array<T> const& b) {
        return a.size() == b.size() && std::equal(a.cbegin(), a.cend(), b.cbegin(), b.cend());
    }
    friend bool operator!=(Dynamic_Array<T> const& a, Dynamic_Array<T> const& b) {
        return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& os, Dynamic_Array<T> const& a) {
        os << "[";
        for (const_reference e : a) {
            os << e << ", ";
        }
        return os << "]";
    }

private:
    size_type m_capacity = 0;
    size_type m_size = 0;
    size_type m_max_size = 0;
    pointer m_data = nullptr;

    constexpr void range_check(size_type index) const noexcept(false) {
        if (index >= size()) {
            throw std::out_of_range("invalid container index: " + std::to_string(index)); // TODO(sdsmith): clearer message?
        }
    }

    void deallocate();
    void allocate(size_type n); // TODO(sdsmith): needed?
    void reallocate(size_type n);
};

} // namespace sds
