// Dijkstra's algorithm for single-source shortest path.
#pragma once

#include "graph/graph.hpp"
#include "binheap/key_val.hpp"
#include "binheap/binheap.hpp"

#include <tuple>
#include <vector>

namespace dijkstra
{

using node   = graph::node;
using weight = graph::weight;

using dist_t = std::vector<weight>;
using prev_t = std::vector<node>;
using prio_t = binheap::binheap<binheap::pair<weight, node>>;

template <typename NodeInfo>
void
relax(const graph::graph<NodeInfo>& g, node current, node successor,
      dist_t& dist, prev_t& prev)
{
    weight old_dist = dist[successor.to_index()];
    weight new_dist =
        dist[current.to_index()] + g.get_weight(current, successor);

    if (new_dist < old_dist) {
        dist[successor.to_index()] = new_dist;
        prev[successor.to_index()] = current;
    }
}

template <typename NodeInfo>
void
relax_successors(const graph::graph<NodeInfo>& g, node current, dist_t& dist,
                 prio_t& prio, prev_t& prev)
{
    for (auto successor : g.get_successors(current)) {
        relax(g, current, successor, dist, prev);
        prio.add({dist[successor.to_index()], successor});
    }
}

template <typename NodeInfo>
std::tuple<dist_t, prev_t>
sssp(const graph::graph<NodeInfo>& g, node start)
{
    auto size = g.size();

    // The least distance found thus far
    dist_t dist(size, graph::INFINITY);

    // Priority queue of least distances found to nodes
    prio_t prio;

    // The previous node on the best path found thus far
    prev_t prev(size, node::INVALID);

    // Set of nodes we've visited
    std::vector<bool> visited(size, false);

    dist[start.to_index()] = 0;
    prio.add({0, start});

    while (!prio.isEmpty()) {
        node current = prio.getMin().value;
        prio.removeMin();

        if (!visited[current.to_index()]) {
            visited[current.to_index()] = true;
            relax_successors(g, current, dist, prio, prev);
        }
    }

    return {dist, prev};
}
}
