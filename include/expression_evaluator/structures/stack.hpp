#pragma once

#include "linked_list.hpp"

namespace expression_evaluator::structures {
template <typename T> class Stack : private LinkedList<T> {
  public:
    /// @brief Add an element to the top of the stack by copy
    void push(const T &value) { LinkedList<T>::push_front(value); }

    /// @brief Add an element to the top of the stack by move
    void push(T &&value) { LinkedList<T>::push_front(std::move(value)); }

    /// @brief Remove and return the top element of the stack
    /// @throws std::runtime_error if the stack is empty
    T pop() { return LinkedList<T>::pop_front(); }

    /// @brief Return a reference to the top element of the stack
    /// @throws std::runtime_error if the stack is empty
    T &top() { return LinkedList<T>::peek_front(); }

    /// @brief Return a const reference to the top element of the stack
    /// @throws std::runtime_error if the stack is empty
    const T &top() const { return LinkedList<T>::peek_front(); }

    /// @brief Return whether the stack is empty
    [[nodiscard]] bool is_empty() const noexcept {
        return LinkedList<T>::is_empty();
    }

    /// @brief Return the number of elements in the stack
    [[nodiscard]] size_t size() const noexcept {
        return LinkedList<T>::get_size();
    }
};
} // namespace expression_evaluator::structures
