#pragma once

#include <cstddef>
#include <iostream>

namespace graph
{

// Represents graph nodes
class node
{
public:
    // Constructs an invalid node.
    node() noexcept;

    // Returns a value to represent this node when used as an
    // array or vector index.
    std::size_t to_index() const noexcept;

    // The invalid node stands for the absence of a node.
    static const node INVALID;

    // The first node, useful for iterating over nodes.
    static const node FIRST;

    // Preincrement
    node& operator++() noexcept;

    // Postincrement
    node operator++(int) noexcept;

    // Node equality
    bool operator==(node) const noexcept;

    // Node inequality
    bool operator!=(node) const noexcept;

private:
    template <typename NodeInfo>
    friend class graph;

    explicit node(std::size_t) noexcept;

    std::size_t value_;
};

inline bool node::operator==(node other) const noexcept
{
    return value_ == other.value_;
}

inline bool node::operator!=(node other) const noexcept
{
    return value_ != other.value_;
}

std::ostream& operator<<(std::ostream& out, node n);

} // namespace graph
