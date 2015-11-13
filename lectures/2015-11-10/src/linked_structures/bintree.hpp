#pragma once

#include <memory>

namespace linked_structures
{

template <typename Element>
class treenode
{
public:
    using handle = std::unique_ptr<treenode<Element>>;
    using elt = Element;

    // Constructs a node with no children
    static handle create(elt);

    // Constructs a node with the given children
    static handle create(elt, handle left, handle right);

    // Gets a reference to the element
    elt& element() { return element_; }
    const elt& element() const { return element_; }

    // Gets a reference to the left subtree
    treenode* left() { return &*left_; }
    const treenode* left() const { return &*left_; }

    // Gets a reference to the right subtree
    treenode* right() { return &*right_; }
    const treenode* right() const { return &*right_; }

private:
    treenode(elt, handle left, handle right);

    Element element_;
    handle left_, right_;
};

template <typename Element>
auto treenode<Element>::create(elt element, handle left, handle right) -> handle
{
    return handle{
        new treenode<elt>{element, std::move(left), std::move(right)}};
}

template <typename Element>
auto treenode<Element>::create(elt element) -> handle
{
    return create(element, nullptr, nullptr);
}

template <typename Element>
treenode<Element>::treenode(elt element, handle left, handle right)
    : element_{element}, left_{std::move(left)}, right_{std::move(right)}
{ }

} // namespace linked_structures
