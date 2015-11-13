// Dijkstra's algorithm for single-source shortest path.

#include "graph/graph.hpp"
#include "binheap/key_val.hpp"
#include "binheap/binheap.hpp"

#include <vector>

namespace dijkstra {

using node   = ::graph::node;
using weight = ::graph::weight;

template <typename NodeInfo>
void
relax(

template <typename NodeInfo>
void
sssp(graph::graph<NodeInfo> g, node start)
{
    size_t size = g.size();

    // The least distance yet found from `start` to each node
    std::vector<weight> dist(size, graph::INFINITY);

    // A priority queue for finding the node with the smallest
    // weight
    binheap::binheap<binheap::pair<weight, node>> prio;

    // The previous node on the shortest path from `start` to each
    // node
    std::vector<node> prev(size, node::INVALID);

    // Whether each node has been visited yet
    std::vector<bool> visited(size, false);

    dist[start.to_index()] = 0;
    prio.add({0, start});

    while (!prio.isEmpty()) {
        auto old_dist = prio.getMin().key;
        auto current = prio.getMin().value;
        prio.removeMin();

        (void)old_dist;
        (void)current;

        if (!visited[current.to_index()]) {
            visited[current.to_index()] = true;

            for (auto succ : g.get_successors(current)) {
                weight new_dist =
                    dist[current.to_index()] + g.get_weight(current, succ);

                if (new_dist < old_dist) {
                    dist[succ.to_index()] = new_dist;
                    prev[succ.to_index()] = current;
                    prio.add({new_dist, succ});
                }
            }
        }
    }
}

}
