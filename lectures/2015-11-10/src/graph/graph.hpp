// Graphs

#include <vector>

namespace graph {

// Stands for the absence of a node.
    constexpr node not_a_node() const { return 0; }

    // The first node.
    constexpr node first_node() const { return 1; }

    size_t to_index(node n) const { return n - first_node(); }

    node from_index(size_t i) const { return i + first_node(); }

// Graphs, where each node carries information of type `NodeInfo`.
template <typename NodeInfo>
class graph
{
public:
    // MEMBER TYPES

    using info   = NodeInfo;
    using node   = size_t;
    using weight = double;

    // CONSTANTS

    static const weight INFINITY;

    // CONSTRUCTORS

    // Creates a new, empty graph.
    graph();

    // Creates a new, empty graph, allocating for `capacity` nodes up front.
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

    // The successors (reachable by non-infinite weight edge) of a node.
    std::vector<node> get_successors(node) const;

private:
    std::vector<info> nodes;
    std::vector<std::vector<weight>> edges;
};

template <typename NodeInfo>
const double graph<NodeInfo>::INFINITY =
    std::numeric_limits<double>::infinity();

template <typename NodeInfo>
graph<NodeInfo>::graph() {}

template <typename NodeInfo>
graph<NodeInfo>::graph(size_t capacity)
    : edges(capacity, std::vector<weight>(capacity, INFINITY))
{
    nodes.reserve(capacity);
}

template <typename NodeInfo>
auto graph<NodeInfo>::last_node() const -> node
{
    return nodes.size();
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
    return nodes[n - 1];
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_weight(node src, node dst) const -> weight
{
    return edges[src - 1][dst - 1];
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_successors(node src) const
    -> std::vector<node>
{
    std::vector<node> result;

    node dst = 1;

    for (auto w : edges[src - 1]) {
        if (w != INFINITY) result.push_back(dst);
        ++dst;
    }

    return result;
}

template <typename NodeInfo>
auto graph<NodeInfo>::add_node(info v) -> node
{
    node new_node = last_node() + 1;

    for (auto& row : edges) {
        if (row.size() < new_node) {
            row.push_back(INFINITY);
        }
    }

    nodes.push_back(v);

    if (edges.size() < new_node) {
        edges.emplace_back(new_node, INFINITY);
    }

    return new_node;
}

template <typename NodeInfo>
void
graph<NodeInfo>::add_edge(node src, node dst, weight w)
{
    edges[src - 1][dst - 1] = w;
}


} // namespace graph
