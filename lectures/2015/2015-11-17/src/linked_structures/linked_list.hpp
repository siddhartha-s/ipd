#pragma once

#include <iostream>
#include <memory>

// Linked lists, with variations.

namespace linked_list
{

namespace unique
{

template <typename T>
class link
{
public:
    using element = T;
    using pointer = std::unique_ptr<link>;

    link(element first, pointer rest)
        : first_{first}, rest_{move(rest)}
    { }

    element& first() { return first_; }
    element const& first() const { return first_; }

    pointer& rest() { return rest_; }
    pointer const& rest() const { return rest_; }

    link* raw() { return this; }
    link const* raw() const { return this; }

private:
    element first_;
    pointer rest_;
};

template <typename T>
using list = typename link<T>::pointer;

template <typename T>
list<T>
cons(T first, list<T> rest)
{
    return std::make_unique<link<T>>(first, move(rest));
}

template <typename T>
size_t length(const list<T>& lst)
{
    size_t result = 0;

    for (const link<T>* hd = lst->raw(); hd != nullptr; hd = hd->rest()->raw())
        ++result;

    return result;
}

template <typename T>
list<T> append(const list<T>& lst1, list<T> lst2)
{
    if (lst1 == nullptr) return lst2;
    return cons(lst1->first(), append<T>(lst1->rest(), move(lst2)));
}

template <typename T>
list<T>&
nth_tail(size_t n, list<T>& lst)
{
    list<T>* hd = &lst;

    while (n > 0) {
        hd = &(*hd)->rest();
        --n;
    }

    return *hd;
}

template <typename T>
const list<T>&
nth_tail(size_t n, const list<T>& lst)
{
    const list<T>* hd = &lst;

    while (n > 0) {
        hd = &(*hd)->rest();
        --n;
    }

    return hd;
}

template <typename T>
T&
nth(size_t n, list<T>& lst)
{
    return nth_tail<T>(n, lst)->first();
}

template <typename T>
const T&
nth(size_t n, const list<T>& lst)
{
    return nth_tail<T>(n, lst)->first();
}

template <typename T>
void
concat(list<T>& front, list<T> back)
{
    list<T>* hd = &front;

    while (*hd != nullptr)
        hd = &(*hd)->rest();

    std::swap(*hd, back);
}

}  // namespace unique

namespace shared
{

template <typename T>
class link
{
public:
    using element = T;
    using pointer = std::shared_ptr<link>;

    link(element first, pointer rest)
        : first_{first}, rest_{rest}
    { }

    element& first() { return first_; }
    element const& first() const { return first_; }

    pointer& rest() { return rest_; }
    pointer const& rest() const { return rest_; }

    link* raw() { return this; }
    link const* raw() const { return this; }

private:
    element first_;
    pointer rest_;
};

template <typename T>
using list = typename link<T>::pointer;

template <typename T>
list<T>
cons(T first, const list<T>& rest)
{
    return std::make_shared<link<T>>(first, rest);
}

template <typename T>
size_t length(const list<T>& lst)
{
    size_t result = 0;

    for (const list<T>* hd = &lst; *hd != nullptr; hd = &(*hd)->rest())
        ++result;

    return result;
}

template <typename T>
list<T> append(const list<T>& lst1, const list<T>& lst2)
{
    if (lst1 == nullptr) return lst2;
    return cons(lst1->first(), append<T>(lst1->rest(), lst2));
}

template <typename T>
list<T>&
nth_tail(size_t n, list<T>& lst)
{
    list<T>* hd = &lst;

    while (n > 0) {
        hd = &(*hd)->rest();
        --n;
    }

    return *hd;
}

template <typename T>
const list<T>&
nth_tail(size_t n, const list<T>& lst)
{
    const list<T>* hd = &lst;

    while (n > 0) {
        hd = &(*hd)->rest();
        --n;
    }

    return hd;
}

template <typename T>
T&
nth(size_t n, list<T>& lst)
{
    return nth_tail<T>(n, lst)->first();
}

template <typename T>
const T&
nth(size_t n, const list<T>& lst)
{
    return nth_tail<T>(n, lst)->first();
}

template <typename T>
void
concat(list<T>& front, list<T>& back)
{
    list<T>* hd = &front;

    while (*hd != nullptr)
        hd = &(*hd)->rest();

    std::swap(*hd, back);
}

}  // namespace shared

}  // namespace linked_list

