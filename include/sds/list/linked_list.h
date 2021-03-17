#pragma once

#include "sds/details/common.h"
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <optional>
#include <memory>


namespace sds
{

template<typename T>
class Linked_List {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    struct Node {
        value_type value = {};
        std::unique_ptr<Node> next = nullptr;

        Node(value_type value) : value(value) {}
    };

    class Iterator {
        Node* m_p = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(Node* p) : m_p(p) {}
        reference operator*() const { return m_p->value; }
        pointer operator->() { return &m_p->value; }
        // prefix inc
        Iterator& operator++() { m_p = m_p->next.get(); return *this; }
        // postfix inc
        Iterator operator++(int) { Iterator it = *this; ++(*this); return it; }

        friend bool operator==(Iterator const& a, Iterator const& b) { return a.m_p == b.m_p; }
        friend bool operator!=(Iterator const& a, Iterator const& b) { return !(a == b); }
    };

    class Const_Iterator {
        Node const* m_p = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T const;
        using pointer = value_type*;
        using reference = value_type&;

        Const_Iterator(Node const* p) : m_p(p) {}
        reference operator*() const { return m_p->value; }
        pointer operator->() { return &m_p->value; }
        // prefix inc
        Const_Iterator& operator++() { m_p = m_p->next.get(); return *this; }
        // postfix inc
        Const_Iterator operator++(int) { Const_Iterator it = *this; ++(*this); return it; }

        friend bool operator==(Const_Iterator const& a, Const_Iterator const& b) { return a.m_p == b.m_p; }
        friend bool operator!=(Const_Iterator const& a, Const_Iterator const& b) { return !(a == b); }
    };

    using iterator = Iterator;
    using const_iterator = Const_Iterator;

    Linked_List() {}

    Linked_List(std::initializer_list<T> l) {
        for (value_type e : l) { push_back(e); }
    }

    /**
     * O(n)
     */
    Linked_List(Linked_List const& o) noexcept(false) {
        std::unique_ptr<Node>* cur = &m_head;
        std::unique_ptr<Node> const* o_cur = &o.m_head;
        while (*o_cur) {
            *cur = std::make_unique<Node>((*o_cur)->value);
            o_cur = &(*o_cur)->next;
            if (!*o_cur) {
                m_tail = cur;
            } else {
                cur = &(*cur)->next;
            }
        }

        m_size = o.m_size;
    }

    /**
       O(1)
    */
    Linked_List(Linked_List&& o) {
        m_tail = o.m_tail;
        m_size = o.m_size;
        m_head = std::move(o.m_head);

        if (m_size <= 1) {
            m_tail = &m_head;
        }
    }

    /**
       O(n)
    */
    ~Linked_List() {
        clear();
    }

    iterator begin() const { return Iterator(m_head.get()); }
    iterator end() const { return Iterator((*m_tail)->next.get()); }
    const_iterator cbegin() const { return Const_Iterator(m_head.get()); }
    const_iterator cend() const { return Const_Iterator((*m_tail)->next.get()); }

    reference front() {
        SDS_ASSERT(m_head && "UB to call front when empty");
        return m_head->value;
    }

    const_reference front() const {
        SDS_ASSERT(m_head && "UB to call front when empty");
        return m_head->value;
    }

    reference back() {
        SDS_ASSERT(*m_tail && "UB to call back when empty");
        return (*m_tail)->value;
    }

    const_reference back() const {
        SDS_ASSERT(*m_tail && "UB to call back when empty");
        return (*m_tail)->value;
    }

    /**
       O(1)
     */
    void push_front(value_type value) {
        std::unique_ptr<Node> node = std::make_unique<Node>(value);
        std::swap(m_head, node);
        std::swap(m_head->next, node);
        m_size++;

        if (size() == 2) {
            m_tail = &m_head->next;
        }
    }

    /**
       O(1)
    */
    void push_back(value_type value) {
        std::unique_ptr<Node> node = std::make_unique<Node>(value);

        if (size() == 0) {
            *m_tail = std::move(node);
        } else {
            (*m_tail)->next = std::move(node);
            m_tail = &(*m_tail)->next;
        }

        m_size++;
    }

    /**
       O(1)
    */
    void pop_front() {
        SDS_ASSERT(!empty());

        std::unique_ptr<Node> node(nullptr);
        std::swap(m_head->next, node);
        std::swap(m_head, node);
        node.reset();
        m_size--;

        if (m_size <= 1) {
            m_tail = &m_head;
        }
    }

    /**
       O(n)
    */
    void pop_back() {
        SDS_ASSERT(!empty());

        m_tail->reset();

        std::unique_ptr<Node>* p1 = &m_head;
        std::unique_ptr<Node>* p2 = (m_head ? &m_head->next : nullptr);
        while (p2 && *p2) {
            p1 = p2;
            p2 = &(*p2)->next;
        }

        m_tail = p1;
        m_size--;
    }

    /**
       O(n)
    */
    void remove(value_type value) {
        std::unique_ptr<Node>* prev = nullptr;
        std::unique_ptr<Node>* cur = &m_head;
        while (*cur) {
            if ((*cur)->value == value) {
                // Update tail
                if (*cur == *m_tail) {
                    m_tail = (prev ? prev : &m_head);
                } else if ((*cur)->next == *m_tail) {
                    // Moving this node will invalidate our pointer to it
                    // TODO(sdsmith): we should probably make sure that pointers to elements are not invalidated when something moves.
                    m_tail = cur;
                }

                // Remove the node
                *cur = std::move((*cur)->next);

                m_size--;
                return;
            }

            prev = cur;
            cur = &(*cur)->next;
        }
    }

    /**
       O(1)
    */
    [[nodiscard]] bool empty() const { return m_size == 0; }

    /**
       O(1)
    */
    [[nodiscard]] size_type size() const { return m_size; }

    /**
       O(n)
    */
    [[nodiscard]] bool contains(value_type value) const {
        Node* cur = m_head.get();
        while (cur) {
            if (cur->value == value) {
                return true;
            }

            cur = cur->next.get();
        }

        return false;
    }

    /**
       O(n)
    */
    void clear() noexcept {
        if (m_head) {
            // Avoid stack overflow from chain destructors
            while (m_head->next) {
                m_head = std::move(m_head->next);
            }
            m_head.reset();
        }

        m_tail = &m_head;
        m_size = 0;
    }

    Linked_List& operator=(Linked_List const& o) {
        SDS_ASSERT(this != &o);
        clear();

        Node const* cur = o.m_head.get();
        while (cur) {
            push_back(cur->value);
            cur = cur->next.get();
        }
        return *this;
    }

    Linked_List& operator=(Linked_List&& o) noexcept {
        SDS_ASSERT(this != &o);
        clear();

        m_tail = o.m_tail;
        m_size = o.m_size;
        m_head = std::move(o.m_head);

        if (m_size <= 1) {
            m_tail = &m_head;
        }
        return *this;
    }

private:
    std::unique_ptr<Node> m_head = nullptr;
    std::unique_ptr<Node>* m_tail = &m_head;
    size_type m_size = 0;
};

} // namesapce sds
