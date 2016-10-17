#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>

namespace ipd {

template<typename T>
class Deque_iterator;

// A deque is a double-ended queue. It is a sequence that supports
// constant-time insertion, observation, and removal at both ends.
template<typename T>
class Deque
{
public:
    // Constructs a new, empty deque.
    Deque();

    // Constructs a deque with the given elements;
    Deque(std::initializer_list<T>);

    // Copy constructor.
    Deque(const Deque&);
    // Copy-assignment operator.
    Deque& operator=(const Deque&);

    // Move constructor.
    Deque(Deque&&);
    // Move assignment operator.
    Deque& operator=(Deque&&);

    // Returns true if the deque is empty.
    bool empty() const;
    // Returns the number of elements in the deque.
    size_t size() const;

    // Returns a reference to the first element of the deque. If the deque is
    // empty then the behavior is undefined.
    const T& front() const;
    T& front();

    // Returns a reference to the last element of the deque. If the deque is
    // empty then the behavior is undefined.
    const T& back() const;
    T& back();

    // Inserts a new element at the front of the deque.
    void push_front(const T&);
    void push_front(T&&);

    // Constructs a new element at the front of the queue.
    template<typename... Args>
    void emplace_front(Args&& ...);

    // Inserts a new element at the back of the deque.
    void push_back(const T&);
    void push_back(T&&);

    // Constructs a new element at the back of the queue.
    template<typename... Args>
    void emplace_back(Args&& ...);

    // Removes the first element of the deque. Undefined if the
    // deque is empty.
    void pop_front();

    // Removes the last element of the deque. Undefined if the
    // deque is empty.
    void pop_back();

    // Removes all elements from the deque.
    void erase();

    // Iterators over Deques.
    using iterator = Deque_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    iterator begin();
    iterator end();
    reverse_iterator rbegin();
    reverse_iterator rend();

    ~Deque();

private:
    struct node_
    {
        explicit node_(const T& value)
                : data(value), prev(nullptr), next(nullptr) {}

        explicit node_(T&& value)
                : data(std::move(value)), prev(nullptr), next(nullptr) {}

        template<typename... Args>
        explicit node_(bool, Args&& ... args)
                : data(std::forward<Args>(args)...), prev(nullptr),
                  next(nullptr) {}

        T data;
        node_* prev;
        node_* next;
    };

    node_* head_ = nullptr;
    node_* tail_ = nullptr;
    size_t size_ = 0;

    void push_front_(node_*);
    void push_back_(node_*);

    friend class Deque_iterator<T>;
};

template<typename T>
class Deque_iterator : public std::iterator<
        std::bidirectional_iterator_tag,
        T>
{
public:
    bool operator==(Deque_iterator);

    T& operator*() const;

    Deque_iterator& operator++();
    Deque_iterator operator++(int);
    Deque_iterator& operator--();
    Deque_iterator operator--(int);

private:
    using node_ = typename Deque<T>::node_;

    node_* current_;
    node_* tail_;

    Deque_iterator(node_* current, node_* tail)
            : current_(current), tail_(tail) {}

    friend class Deque<T>;
};

///
/// IMPLEMENTATIONS
///

template<typename T>
Deque<T>::Deque() {}

template<typename T>
Deque<T>::Deque(std::initializer_list<T> args)
{
    for (const auto& arg : args)
        push_back(arg);
}

template<typename T>
Deque<T>::Deque(const Deque& other)
{
    for (node_* curr = other.head_; curr != nullptr; curr = curr->next)
        push_back(curr->data);
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque& other)
{
    erase();

    for (node_* curr = other.head_; curr != nullptr; curr = curr->next)
        push_back(curr->data);

    return *this;
}

template<typename T>
Deque<T>::Deque(Deque&& other)
        : head_(other.head_), tail_(other.tail_), size_(other.size_)
{
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
}

template<typename T>
Deque<T>& Deque<T>::operator=(Deque&& other)
{
    erase();

    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;

    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
}

template<typename T>
bool Deque<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
size_t Deque<T>::size() const
{
    return size_;
}

template<typename T>
const T& Deque<T>::front() const
{
    return head_->data;
}

template<typename T>
T& Deque<T>::front()
{
    return head_->data;
}

template<typename T>
const T& Deque<T>::back() const
{
    return tail_->data;
}

template<typename T>
T& Deque<T>::back()
{
    return tail_->data;
}

template<typename T>
void Deque<T>::push_front_(node_* new_node)
{
    new_node->next = head_;
    if (head_ == nullptr)
        tail_ = new_node;
    else
        head_->prev = new_node;

    head_ = new_node;
    ++size_;
}

template<typename T>
void Deque<T>::push_front(const T& value)
{
    push_front_(new node_(value));
}

template<typename T>
void Deque<T>::push_front(T&& value)
{
    push_front_(new node_(std::move(value)));
}

template<typename T>
template<typename... Args>
void Deque<T>::emplace_front(Args&& ... args)
{
    push_front_(new node_(true, std::forward(args)...));
}

template<typename T>
void Deque<T>::push_back_(node_* new_node)
{
    new_node->prev = tail_;
    if (tail_ == nullptr)
        head_ = new_node;
    else
        tail_->next = new_node;

    tail_ = new_node;
    ++size_;
}

template<typename T>
void Deque<T>::push_back(const T& value)
{
    push_back_(new node_(value));
}

template<typename T>
void Deque<T>::push_back(T&& value)
{
    push_back_(new node_(std::move(value)));
}

template<typename T>
template<typename... Args>
void Deque<T>::emplace_back(Args&& ... args)
{
    push_back_(new node_(true, std::forward<Args>(args)...));
}

template<typename T>
void Deque<T>::pop_front()
{
    node_* new_head = head_->next;
    delete head_;
    head_ = new_head;

    if (head_ == nullptr) tail_ = nullptr;
    --size_;
}

template<typename T>
void Deque<T>::pop_back()
{
    node_* new_tail = tail_->prev;
    delete tail_;
    tail_ = new_tail;

    if (tail_ == nullptr) head_ = nullptr;
    --size_;
}

template<typename T>
void Deque<T>::erase()
{
    while (!empty()) pop_front();
}

template<typename T>
Deque<T>::~Deque()
{
    erase();
}

template<typename T>
auto Deque<T>::begin() -> iterator
{
    return iterator(head_, tail_);
}

template<typename T>
auto Deque<T>::end() -> iterator
{
    return iterator(nullptr, tail_);
}

template<typename T>
auto Deque<T>::rbegin() -> reverse_iterator
{
    return std::reverse_iterator<iterator>(iterator(nullptr, tail_));
}

template<typename T>
auto Deque<T>::rend() -> reverse_iterator
{
    return std::reverse_iterator<iterator>(iterator(head_, tail_));
}

template<typename T>
bool operator!=(Deque_iterator<T>, Deque_iterator<T>);

template<typename T>
bool Deque_iterator<T>::operator==(Deque_iterator other)
{
    return current_ == other.current_ && tail_ == other.tail_;
}

template<typename T>
T& Deque_iterator<T>::operator*() const
{
    return current_->data;
}

template<typename T>
Deque_iterator<T>& Deque_iterator<T>::operator++()
{
    current_ = current_->next;
    return *this;
}

template<typename T>
Deque_iterator<T> Deque_iterator<T>::operator++(int)
{
    Deque_iterator result(*this);
    ++*this;
    return result;
}

template<typename T>
Deque_iterator<T>& Deque_iterator<T>::operator--()
{
    if (current_ == nullptr)
        current_ = tail_;
    else
        current_ = current_->prev;

    return *this;
}

template<typename T>
Deque_iterator<T> Deque_iterator<T>::operator--(int)
{
    Deque_iterator result(*this);
    --*this;
    return result;
}

template<typename T>
bool operator!=(Deque_iterator<T> i, Deque_iterator<T> j)
{
    return !(i == j);
}

}
