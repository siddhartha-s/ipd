#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>

// declare one of the test classes so it can be our friend
class TestInsertFinds;

namespace ipd {

// A simple binary search tree using leaf-insertion (no balance).
template<typename T>
class Bst
{
public:
    // Constructs an empty tree.
    Bst();
    // Constructs a tree containing the given elements.
    Bst(std::initializer_list<T>);

    // Is this tree empty?
    bool empty() const;
    // Returns the number of elements in the tree.
    size_t size() const;

    // Does the tree contain the given value?
    bool contains(const T&) const;
    // Inserts an element into the tree, if absent.
    void insert(const T&);
    // Removes an element from the tree, if present.
    void remove(const T&);

    bool bst_invariant_holds();

private:
    struct node_ {
        node_(const T& value)
          : data(value), left(nullptr), right(nullptr) {}

        T data;
        std::unique_ptr<node_> left;
        std::unique_ptr<node_> right;
    };

    std::unique_ptr<node_>   root_;
    size_t size_;

    std::unique_ptr<node_>* find_to_remove(const T& key);

    static std::unique_ptr<node_>* find_next_largest(std::unique_ptr<node_>* to_remove);

    bool bounded(node_* node, T lo, bool lo_inf, T hi, bool hi_inf);

    friend TestInsertFinds;
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
    assert ((size_ == 0) == (root_ == nullptr));
    return size_ == 0;
}

template<typename T>
size_t Bst<T>::size() const
{
    return size_;
}

template<typename T>
bool Bst<T>::contains(const T& key) const
{
    node_* curr = &*root_;

    while (curr != nullptr) {
        if (key < curr->data) curr = &*curr->left;
        else if (curr->data < key) curr = &*curr->right;
        else return true;
    }

    return false;
}

template<typename T>
void Bst<T>::insert(const T& key)
{
    std::unique_ptr<node_>* curr = &root_;

    while (*curr != nullptr) {
        if (key < (*curr)->data) curr = &(*curr)->left;
        else if ((*curr)->data < key) curr = &(*curr)->right;
        else return;
    }

    *curr = std::make_unique<node_>(key);
    ++size_;
}

template<typename T>
std::unique_ptr<typename Bst<T>::node_>* Bst<T>::find_to_remove(const T& key)
{
    std::unique_ptr<node_>* ret = &root_;
    while ((*ret) != nullptr) {
        if ((*ret)->data < key)
            ret = &(*ret)->right;
        else if (key < (*ret)->data)
            ret = &(*ret)->left;
        else break;
    }
    return ret;
}

// the next largest node (if there is one), will be the leftmost child of the
// right child of the given node. If there is no right child, then the input
// node is the largest (so there is no next largest).
template<typename T>
typename std::unique_ptr<typename Bst<T>::node_>*
        Bst<T>::find_next_largest(std::unique_ptr<node_>* to_remove) {
    assert((*to_remove) != nullptr);
    assert((*to_remove)->right != nullptr);
    std::unique_ptr<node_>* ret = &((*to_remove)->right);
    while ((*ret) -> left != nullptr)
        ret = &(*ret)->left;
    return ret;
}

template<typename T>
void Bst<T>::remove(const T& key)
{
    std::unique_ptr<node_>* to_remove = find_to_remove(key);

    // if the node wasn't in the tree, just return. nothing to remove
    if (*to_remove == nullptr) return;

    // if the right node is null, then we can replace it
    // by its left child.
    if ((*to_remove)->right == nullptr) {
        *to_remove = std::move((*to_remove)->left);
    } else {
        // Otherwise, we find the successor node and then swap the contents with
        // the successor node and delete the successor by replacing it with
        // its right child.
        std::unique_ptr<node_>* to_swap = find_next_largest(to_remove);
        std::swap((*to_remove)->data, (*to_swap)->data);
        *to_swap = std::move((*to_swap)->right);
    }
    size_--;
}

template<typename T>
bool Bst<T>::bst_invariant_holds()
{
    if (root_ == nullptr) return true;
    return bounded(&*root_, root_->data, true, root_->data, true);

}

template<typename T>
bool Bst<T>::bounded(node_ *node, T lo, bool lo_inf, T hi, bool hi_inf)
{
    if (node == nullptr) return true;
    if (!lo_inf && node->data < lo) return false;
    if (!hi_inf && hi < node->data) return false;
    return bounded(&*node->left, lo, lo_inf, node->data, false) &&
           bounded(&*node->right, node->data, false, hi, hi_inf);
}
}
