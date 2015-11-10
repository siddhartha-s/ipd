// Graphs

#include <vector>

namespace graph {

// Graphs, where each node carries information of type `NodeInfo`.
template <typename NodeInfo>
class graph
{
public:
    using node   = size_t;
    using weight = double;
    using info   = NodeInfo;

    static const weight INFINITY;

    graph();

    // Allocates for a graph with `capacity` nodes up front.
    graph(size_t capacity);

    node add_node(info);
    void add_edge(node src, node dst, weight);

    size_t size() const;
    info get_info(node) const;
    weight get_weight(node src, node dst) const;
    std::vector<node> get_successors(node) const;

private:
    std::vector<info> nodes;
    std::vector<std::vector<weight>> edges;
};

template <typename NodeInfo>
graph<NodeInfo>::graph() {}

template <typename NodeInfo>
graph<NodeInfo>::graph(size_t capacity)
    : edges(capacity, std::vector<weight>(capacity, INFINITY))
{
    nodes.reserve(capacity);
}


template <typename NodeInfo>
const double graph<NodeInfo>::INFINITY =
    std::numeric_limits<double>::infinity();

template <typename NodeInfo>
auto graph<NodeInfo>::add_node(info v) -> node
{
    nodes.push_back(v);
    return nodes.size() - 1;
}

template <typename NodeInfo>
void
graph<NodeInfo>::add_edge(node src, node dst, weight w)
{
    while (edges.size() <= src) {
        edges.emplace_back();
    }

    while (edges[src].size() <= dst) {
        edges[src].push_back(INFINITY);
    }

    edges[src][dst] = w;
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
    return nodes[n];
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_weight(node src, node dst) const -> weight
{
    if (!(src < edges.size() && dst < edges[src].size())) {
        return INFINITY;
    } else {
        return edges[src][dst];
    }
}

template <typename NodeInfo>
auto graph<NodeInfo>::get_successors(node src) const
    -> std::vector<node>
{
    std::vector<node> result;

    if (src < edges.size()) {
        node dst = 0;

        for (auto w : edges[src]) {
            if (w != INFINITY)
                result.push_back(dst);

            ++dst;
        }
    }

    return result;
}

} // namespace graph
