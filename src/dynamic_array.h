#pragma once

#include <initializer_list>
#include <iterator>


namespace sds
{

/*
  TODO(sdsmith): https://en.cppreference.com/w/cpp/named_req/ContiguousIterator
  - check reqs in unittest
  https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
  - check in unittests
*/


template<typename T>
class Dynamic_Array {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    class Iterator {
        pointer m_p = nullptr;

    public:
        // @c++20
        //using iterator_category = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(pointer p) : m_p(p) {}
        reference operator*() const { return *m_p; }
        pointer operator->() { return m_p; }
        Iterator& operator++() { m_p++; return *this; }
        Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }

        friend bool operator==(Iterator const& a, Iterator const& b) { return a.m_p == b.m_p; }
        friend bool operator!=(Iterator const& a, Iterator const& b) { return !(a == b); }
    };

    class Const_Iterator {
        pointer m_p = nullptr;

    public:
        // @c++20
        //using iterator_category = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T const;
        using pointer = value_type*;
        using reference = value_type&;

        Const_Iterator(const_pointer p) : m_p(p) {}
        const_reference operator*() const { return *m_p; }
        const_pointer operator->() { return m_p; }
        Const_Iterator& operator++() { m_p++; return *this; }
        Const_Iterator operator++(int) { Const_Iterator it = *this; ++(*this); return it; }

        friend bool operator==(Const_Iterator const& a, Const_Iterator const& b) { return a.m_p == b.m_p; }
        friend bool operator!=(Const_Iterator const& a, Const_Iterator const& b) { return !(a == b); }
    };

    using iterator = Iterator;
    using const_iterator = Const_Iterator;

    Dynamic_Array();
    Dynamic_Array(std::initializer_list<T> l);
    Dynamic_Array(Dynamic_Array const& o);
    Dynamic_Array(Dynamic_Array&& o);
    ~Dynamic_Array();

    Dynamic_Array& operator=(Dynamic_Array const&) = delete;
    Dynamic_Array& operator=(Dynamic_Array &&);

    iterator begin() const { return Iterator(m_data); }
    iterator end() const { return Iterator(m_data + m_size); }
    const_iterator cbegin() const { return Const_Iterator(m_data); }
    const_iterator cend() const { return Const_Iterator(m_data + m_size); }

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference data();
    const_reference data() const;

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

    constexpr reference at(size_type pos);
    constexpr const_reference at(size_type pos) const;
    /* no throw */
    constexpr reference operator[](size_type pos);
    constexpr const_reference operator[](size_type pos) const;

    constexpr bool empty() const;
    constexpr size_t size() const;
    constexpr size_t capacity() const;
    constexpr void reserve(size_t);
    // TODO(sdsmith): shrink_to_fit

    constexpr void swap(Dynamic_Array& o);

    friend std::ostream& operator<<(std::ostream& os, Dynamic_Array const& a) {
        os << "[";
        for (const_reference e : a) {
            os << e << ", ";
        }
        return os << "]";
    }

private:
    size_type m_capacity = 0;
    size_type m_size = 0;
    pointer m_data = nullptr;
};

} // namespace sds
