#pragma once

#include "linked_list.hpp"

namespace expression_evaluator::structures {
template <typename T> class Queue : private LinkedList<T> {
  public:
    /// @brief Add an element to the end of the queue by copy
    void enqueue(const T &value) { LinkedList<T>::push_back(value); }

    /// @brief Add an element to the end of the queue by move
    void enqueue(T &&value) { LinkedList<T>::push_back(std::move(value)); }

    /// @brief Remove and return the element at the front of the queue
    T dequeue() { return LinkedList<T>::pop_front(); }

    /// @brief Return a reference to the front element of the queue
    T &front() { return LinkedList<T>::peek_front(); }

    /// @brief Return a const reference to the front element of the queue
    const T &front() const { return LinkedList<T>::peek_front(); }

    /// @brief Returns whether the queue is empty
    [[nodiscard]] bool is_empty() const noexcept {
        return LinkedList<T>::is_empty();
    }

    /// @brief Returns the number of elements in the queue
    [[nodiscard]] size_t size() const noexcept {
        return LinkedList<T>::get_size();
    }
};
} // namespace expression_evaluator::structures
