#pragma once

#include <stdexcept>

template <typename T>
class Queue
{
public:
    Queue();

    void enqueue(const T&);
    T dequeue();
    bool empty() const;

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

private:
    struct node_;
    using link_t = node_*;

    link_t head_;
    link_t tail_;
};

template <typename T>
Queue<T>::Queue()
        : head_{nullptr}, tail_{nullptr}
{ }

template <typename T>
struct Queue<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    link_t next;
};

template <typename T>
void Queue<T>::enqueue(const T& elt)
{
    link_t new_node = new node_(elt);

    if (head_ == nullptr) {
        head_ = new_node;
    } else {
        tail_->next = new_node;
    }

    tail_ = new_node;
}

template <typename T>
T Queue<T>::dequeue()
{
    if (empty())
        throw std::logic_error("LL_queue::dequeue(): empty queue");

    T result = head_->element;
    link_t old_head = head_;
    head_ = head_->next;
    if (head_ == nullptr) tail_ = nullptr;

    delete old_head;

    return result;
}

template <typename T>
bool Queue<T>::empty() const
{
    return head_ == nullptr;
}

