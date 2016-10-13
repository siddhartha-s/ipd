#pragma once

#include <initializer_list>
#include <memory>

namespace ipd {

template<typename T>
class Bst
{
public:
    Bst();
    Bst(std::initializer_list<T>);

    bool empty() const;
    size_t size() const;

    void insert(const T&);
    void remove(const T&);
    bool contains(const T&) const;

private:
    struct node_;
    using ptr_ = std::unique_ptr<node_>;

    struct node_
    {
        node_(const T& value)
                : data(value), left(nullptr), right(nullptr), size(1) {}

        T      data;
        ptr_   left;
        ptr_   right;
        size_t size;
    };

    ptr_   root_;
    size_t size_;
};

template<typename T>
Bst<T>::Bst()
        : root_(nullptr), size_(0) {}

template<typename T>
Bst<T>::Bst(std::initializer_list<T> init)
        : Bst()
{
    for (const auto& each : init)
        insert(each);
}

template<typename T>
bool Bst<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
size_t Bst<T>::size() const
{
    return size_;
}

template<typename T>
void Bst<T>::insert(const T& key)
{
    ptr_* curr = &root_;

    while (*curr != nullptr) {
        if (key < (*curr)->data) curr = &(*curr)->left;
        else if (key > (*curr)->data) curr = &(*curr)->right;
        else return;
    }

    *curr = std::make_unique<node_>(key);
    ++size_;
}

template<typename T>
void Bst<T>::remove(const T& key)
{
    ptr_* curr = &root_;

    while (*curr != nullptr) {
        if (key < (*curr)->data) curr = &(*curr)->left;
        else if (key > (*curr)->data) curr = &(*curr)->right;
        else {
            if ((*curr)->right == nullptr) {
                *curr = std::move((*curr)->left);
            } else {
                ptr_* other = &(*curr)->right;
                while ((*other)->left != nullptr) other = &(*other)->left;
                std::swap((*other)->data, (*curr)->data);
                *other = std::move((*other)->right);
            }

            size_--;
            return;
        }
    }
}

template<typename T>
bool Bst<T>::contains(const T& key) const
{
    node_* curr = &*root_;

    while (curr != nullptr) {
        if (key < curr->data) curr = &*curr->left;
        else if (key > curr->data) curr = &*curr->right;
        else return true;
    }

    return false;
}

}
