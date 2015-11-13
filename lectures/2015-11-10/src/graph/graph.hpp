// Graphs

#pragma once

#include "node.hpp"
#include <vector>

namespace graph
{

using node   = node;
using weight = double;

// The infinite weight.
constexpr weight INFINITY = std::numeric_limits<weight>::infinity();

// Graphs, where each node carries information of type `NodeInfo`.
template <typename NodeInfo>
class graph
{
public:
    // MEMBER TYPES

    using info   = NodeInfo;
    using node   = node;
    using weight = weight;

    // MEMBER CONSTANTS

    static const weight INFINITY;

    // CONSTRUCTORS

    // Creates a new, empty graph.
    graph();

    // Creates a new, empty graph, allocating for `capacity` nodes
    // up front.
    graph(size_t capacity);

    // MUTATORS

    // Adds a node with the given info, returning its name.
    node add_node(info);

    // Adds an edges from `src` to `dst` with a given weight.
    void add_edge(node src, node dst, weight);

    // OBSERVERS

    // The last node.
    node last_node() const;

    // The number of nodes in the graph.
    size_t size() const;

    // The information associated with a node.
    info get_info(node) const;

    // The weight of the edge between `src` and `dst`.
    weight get_weight(node src, node dst) const;

    // The successors (non-infinite weight edge) of a node.
    std::vector<node> get_successors(node) const;

    // The predecessors (non-infinite weight edge) of a node.
    std::vector<node> get_predecessors(node) const;

private:
    std::vector<info> nodes;
    std::vector<std::vector<weight>> edges;
};

template <typename NodeInfo>
const weight graph<NodeInfo>::INFINITY = ::graph::INFINITY;

template <typename NodeInfo>
graph<NodeInfo>::graph()
{ }

template <typename NodeInfo>
graph<NodeInfo>::graph(size_t capacity)
    : edges(capacity, std::vector<weight>(capacity, INFINITY))
{
    nodes.reserve(capacity);
}

template <typename NodeInfo>
auto graph<NodeInfo>::last_node() const -> node
{
    return node{size()};
}

template <typename NodeInfo>
size_t
graph<NodeInfo>::size() const
{
    return nodes.size();
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_info(node n) const -> info
{
    return nodes[n.to_index()];
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_weight(node src, node dst) const -> weight
{
    return edges[src.to_index()][dst.to_index()];
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_successors(node src) const -> std::vector<node>
{
    std::vector<node> result;

    node dst = node::FIRST;

    for (auto w : edges[src.to_index()]) {
        if (w != INFINITY)
            result.push_back(dst);
        ++dst;
    }

    return result;
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_predecessors(node dst) const -> std::vector<node>
{
    std::vector<node> result;

    node src = node::FIRST;

    for (const auto& row : edges) {
        if (row[dst.to_index()] != INFINITY)
            result.push_back(src);
        ++src;
    }

    return result;
}

template <typename NodeInfo>
auto graph<NodeInfo>::add_node(info v) -> node
{
    size_t new_size = size() + 1;

    for (auto& row : edges) {
        if (row.size() < new_size) {
            row.push_back(INFINITY);
        }
    }

    nodes.push_back(v);

    if (edges.size() < new_size) {
        edges.emplace_back(new_size, INFINITY);
    }

    return node{new_size};
}

template <typename NodeInfo>
void
graph<NodeInfo>::add_edge(node src, node dst, weight w)
{
    edges[src.to_index()][dst.to_index()] = w;
}

}  // namespace graph
