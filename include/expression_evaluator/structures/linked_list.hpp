#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace expression_evaluator::structures {
template <typename T> class LinkedList {
  private:
    struct Node {
        T data;
        Node *next;

        // Forwarding-reference constructor, to preserve lvalue/rvalue-ness so
        // push_* can copy (lvalues) or move (rvalues) into Node::data
        template <typename U>
        explicit Node(U &&value)
            : data(std::forward<U>(value)), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    size_t size;

  public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() { clear(); }

    // Do not support copy semantics for simplicity
    LinkedList(const LinkedList &) = delete;
    LinkedList &operator=(const LinkedList &) = delete;

    LinkedList(LinkedList &&other) noexcept
        : head(other.head), tail(other.tail), size(other.size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    LinkedList &operator=(LinkedList &&other) noexcept {
        if (this == &other)
            return *this;

        clear();
        head = other.head;
        tail = other.tail;
        size = other.size;
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
        return *this;
    }

    /// @brief Add an element to the front of the list by copy
    void push_front(const T &value) {
        Node *new_node = new Node(value);
        new_node->next = head;

        head = new_node;
        if (tail == nullptr)
            tail = head;

        size++;
    }

    /// @brief Add an element to the front of the list by move
    void push_front(T &&value) {
        Node *new_node = new Node(std::move(value));
        new_node->next = head;

        head = new_node;
        if (tail == nullptr)
            tail = head;

        size++;
    }

    /// @brief Add an element to the end of the list by copy
    void push_back(const T &value) {
        Node *new_node = new Node(value);
        if (tail == nullptr)
            head = tail = new_node;
        else {
            tail->next = new_node;
            tail = new_node;
        }

        size++;
    }

    /// @brief Add an element to the end of the list by move
    void push_back(T &&value) {
        Node *new_node = new Node(std::move(value));
        if (tail == nullptr)
            head = tail = new_node;
        else {
            tail->next = new_node;
            tail = new_node;
        }

        size++;
    }

    /// @brief Remove and return the front element of the list
    /// @throws std::runtime_error if the list is empty
    T pop_front() {
        if (is_empty())
            throw std::runtime_error("Cannot pop from empty list");

        Node *temp = head;
        T value = std::move(head->data);
        head = head->next;
        if (head == nullptr)
            tail = nullptr;

        delete temp;
        size--;
        return value;
    }

    /// @brief Return a reference to the front element of the list
    /// @throws std::runtime_error if the list is empty
    T &peek_front() {
        if (is_empty())
            throw std::runtime_error("Cannot peek from empty list");

        return head->data;
    }

    /// @brief Return a const reference to the front element of the list
    /// @throws std::runtime_error if the list is empty
    const T &peek_front() const {
        if (is_empty())
            throw std::runtime_error("Cannot peek from empty list");

        return head->data;
    }

    /// @brief Returns whether the list is empty
    [[nodiscard]] bool is_empty() const noexcept { return head == nullptr; }

    /// @brief Returns the number of elements in the list
    [[nodiscard]] size_t get_size() const noexcept { return size; }

    /// @brief Clear all elements from the list
    void clear() noexcept {
        while (head != nullptr) {
            Node *next = head->next;
            delete head;
            head = next;
        }
        tail = nullptr;
        size = 0;
    }
};
} // namespace expression_evaluator::structures
