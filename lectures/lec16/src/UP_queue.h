#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
class Queue
{
public:
    void enqueue(const T&);
    T dequeue();
    bool empty() const;

private:
    struct node_;

    std::unique_ptr<node_> head_;
    node_* tail_;
};

template <typename T>
struct Queue<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    std::unique_ptr<node_> next;
};

template <typename T>
void Queue<T>::enqueue(const T& elt)
{
    auto new_node = std::make_unique<node_>(elt);
    node_* new_tail = &*new_node;

    if (head_ == nullptr) {
        head_ = std::move(new_node);
    } else {
        tail_->next = std::move(new_node);
    }

    tail_ = new_tail;
}

template <typename T>
T Queue<T>::dequeue()
{
    if (empty())
        throw std::logic_error("LL_queue::dequeue(): empty queue");

    T result = head_->element;
    head_ = std::move(head_->next);
    if (head_ == nullptr) tail_ = nullptr;

    return result;
}

template <typename T>
bool Queue<T>::empty() const
{
    return head_ == nullptr;
}


