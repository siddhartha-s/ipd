#pragma once

#include <stdexcept>
#include <memory>


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

    node_* tail_;
};

template <typename T>
RingBuffer<T>::RingBuffer()
        : tail_{nullptr}
{ }

template <typename T>
struct RingBuffer<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    std::unique_ptr<node_> next;
};

template <typename T>
void RingBuffer<T>::insert(const T& elt)
{
    std::unique_ptr<node_> new_node = std::make_unique<node_>(elt);
    if (tail_ == nullptr) {
        tail_ = new_node.get();
        std::swap(new_node, new_node->next);
    } else {
        std::swap(new_node,tail_->next);
        std::swap(new_node->next,new_node);
    }
}

template <typename T>
T& RingBuffer<T>::remove()
{
    if (empty())
        throw std::logic_error("RP_RingBuffer::dequeue(): empty ring buffer");

    std::unique_ptr<node_> old_head = nullptr;
    std::swap(tail_->next,old_head);
    std::swap(old_head->next,tail_->next);
    return old_head->element;
}

template <typename T>
void RingBuffer<T>::rotate()
{
    if (tail_ != nullptr)
        tail_ = tail_->next.get();
}


template <typename T>
bool RingBuffer<T>::empty() const
{
    return tail_ == nullptr;
}
