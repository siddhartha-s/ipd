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
    using link_t = std::shared_ptr<node_>;

    link_t head_;
    link_t tail_;
};

template <typename T>
struct Queue<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    link_t next;
};

template <typename T>
void Queue<T>::enqueue(const T& elt)
{
    link_t new_node = std::make_shared<node_>(elt);

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
    head_ = head_->next;
    if (head_ == nullptr) tail_ = nullptr;

    return result;
}

template <typename T>
bool Queue<T>::empty() const
{
    return head_ == nullptr;
}

