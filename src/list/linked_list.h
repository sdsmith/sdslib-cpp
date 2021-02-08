#pragma once

#include <initializer_list>
#include <optional>
#include <memory>

namespace sds {

template<typename ElT>
class Linked_List {
public:
    struct Node {
        ElT value = {};
        std::unique_ptr<Node> next = nullptr;

        Node(ElT value) : value(value) {}
    };

    Linked_List() {}

    Linked_List(std::initializer_list<ElT> l) {
        for (ElT e : l) { push_back(e); }
    }

    /**
       O(n)
     */
    Linked_List(Linked_List const& o) {
        clear();

        std::unique_ptr<Node>* cur = &m_head;
        std::unique_ptr<Node> const* o_cur = &o.m_head;
        while (*o_cur) {
            *cur = std::make_unique<Node>((*o_cur)->value);
            cur = &(*cur)->next;
            o_cur = &(*o_cur)->next;
        }

        m_tail = cur;
        m_size = o.m_size;
    }

    /**
       O(1)
    */
    Linked_List(Linked_List&& o) {
        clear();

        m_head = std::move(o.m_head);
        m_tail = o.m_tail;
        m_size = o.m_size;
    }

    /**
       O(n)
    */
    ~Linked_List() {
        clear();
    }

    std::optional<ElT> front() const {
        return (m_head ? std::make_optional<ElT>(m_head->value) : std::nullopt);
    }

    std::optional<ElT> back() const {
        return (*m_tail ? std::make_optional<ElT>((*m_tail)->value) : std::nullopt);
    }

    /**
       O(1)
     */
    void push_front(ElT value) {
        std::unique_ptr<Node> node = std::make_unique<Node>(value);
        std::swap(m_head, node);
        std::swap(m_head->next, node);
        m_size++;

        if (m_size == 1) {
            m_tail = &m_head;
        }
    }

    /**
       O(1)
    */
    void push_back(ElT value) {
        std::unique_ptr<Node> node = std::make_unique<Node>(value);
        (*m_tail)->next = std::move(node);
        m_tail = &(*m_tail)->next;
        m_size++;
    }

    /**
       O(1)
    */
    void pop_front() {
        std::unique_ptr<Node> node(nullptr);
        std::swap(m_head->next, node);
        std::swap(m_head, node);
        node.reset();
        m_size--;
    }

    /**
       O(n)
    */
    void pop_back() {
        m_tail->reset();
        std::unique_ptr<Node>* p1 = &m_head;
        std::unique_ptr<Node>* p2 = (m_head ? &m_head.next : nullptr);

        while (*p2) {
            p1 = p2;
            p2 = &p2->next;
        }

        m_tail = p1;
    }

    /**
       O(n)
    */
    void remove(ElT value) {
        std::unique_ptr<Node>* cur = &m_head;
        while (*cur) {
            if (cur->value == value) {
                *cur = std::move(cur->next);
                return;
            }

            cur = &cur->next;
        }
    }

    /**
       O(1)
    */
    [[nodiscard]] bool empty() const { return m_size == 0; }

    /**
       O(1)
    */
    [[nodiscard]] size_t size() const { return m_size; }

    /**
       O(n)
    */
    [[nodiscard]] bool contains(ElT value) const {
        Node* cur = &m_head;
        while (*cur) {
            if (cur->value == value) {
                return true;
            }
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
                m_head = std::move(m_head->next->next);
            }
            m_head.reset();
        }

        m_tail = &m_head;
        m_size = 0;
    }

    Linked_List& operator=(Linked_List const& o) {
        clear();

        std::unique_ptr<Node>* cur = &o.m_head;
        int ct = o.size();

        while (*cur) {
            push_back(cur->value);

            ct--;
            if (ct == 0) {
                // at the tail element
                m_tail = *cur;
            }
        }

        return *this;
    }

    Linked_List& operator=(Linked_List&& o) {
        clear();
        m_head = std::move(o.m_head);
        m_tail = std::move(o.m_tail);
        m_size = o.m_size;
        return *this;
    }

private:
    std::unique_ptr<Node> m_head = nullptr;
    std::unique_ptr<Node>* m_tail = &m_head;
    size_t m_size = 0;
};

} // namesapce sds
