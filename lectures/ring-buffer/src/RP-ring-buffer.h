#pragma once

#include <stdexcept>

template <typename T>
class RingBuffer
{
public:
    RingBuffer();

    // adds a new element to the ring buffer
    void insert(const T&);

    // rotates the buffer
    void rotate();

    // removes frontmost element
    // pre: !empty()
    T& remove();

    bool empty() const;

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    ~RingBuffer();

private:
    struct node_;
    using link_t = node_*;

    link_t tail_;
};

template <typename T>
RingBuffer<T>::RingBuffer()
        : tail_{nullptr}
{ }

template <typename T>
struct RingBuffer<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    link_t next;
};

template <typename T>
RingBuffer<T>::~RingBuffer()
{
    while (!empty()) {
        remove();
    }
}

template <typename T>
void RingBuffer<T>::insert(const T& elt)
{
    link_t new_node = new node_(elt);
    if (tail_ == nullptr) {
        tail_ = new_node;
        tail_->next = tail_;
    } else {
        new_node->next = tail_->next;
        tail_->next = new_node;
    }
}

template <typename T>
T& RingBuffer<T>::remove()
{
    if (empty())
        throw std::logic_error("RP_RingBuffer::dequeue(): empty ring buffer");

    link_t old_head = tail_->next;
    T& res = old_head->element;

    if (old_head->next == old_head) {
        tail_=nullptr;
    } else {
        tail_->next = old_head->next;
    }

    delete old_head;
    return res;
}

template <typename T>
void RingBuffer<T>::rotate()
{
    if (tail_ != nullptr)
        tail_ = tail_ -> next;
}


template <typename T>
bool RingBuffer<T>::empty() const
{
    return tail_ == nullptr;
}
