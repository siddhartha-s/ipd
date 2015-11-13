#pragma once

// Dijkstra's algorithm for single-source shortest path.

#include "graph/graph.hpp"
#include "binheap/key_val.hpp"
#include "binheap/binheap.hpp"

#include <tuple>
#include <vector>

namespace dijkstra {

using node   = graph::node;
using weight = graph::weight;

using dist_t = std::vector<weight>;
using prev_t = std::vector<node>;
using prio_t = binheap::binheap<binheap::pair<weight, node>>;

template <typename NodeInfo>
std::tuple<dist_t, prev_t>
relax(graph::graph<NodeInfo> g, node current, node succ,
      dist_t dist, prev_t prev)
{
    weight old_dist = dist[succ.to_index()];
    weight new_dist = dist[current.to_index()] + g.get_weight(current, succ);

    if (new_dist < old_dist) {
        dist[succ.to_index()] = new_dist;
        prev[succ.to_index()] = current;
    }

    return {dist, prev};
}

template <typename NodeInfo>
std::tuple<dist_t, prev_t, prio_t>
relax_successors(graph::graph<NodeInfo> g, node current,
                 dist_t dist, prev_t prev, prio_t prio)
{
    for (auto succ : g.get_successors(current)) {
        std::tie(dist, prev) = relax(g, current, succ, dist, prev);
        prio.add({dist[succ.to_index()], succ});
    }

    return {dist, prev, prio};
}

template <typename NodeInfo>
std::tuple<dist_t, prev_t>
sssp(graph::graph<NodeInfo> g, node start)
{
    size_t size = g.size();

    // The least distance yet found from `start` to each node
    dist_t dist(size, graph::INFINITY);

    // A priority queue for finding the node with the smallest
    // weight
    prio_t prio;

    // The previous node on the shortest path from `start` to each
    // node
    prev_t prev(size, node::INVALID);

    // Whether each node has been visited yet
    std::vector<bool> visited(size, false);

    dist[start.to_index()] = 0;
    prio.add({0, start});

    while (!prio.isEmpty()) {
        auto current = prio.getMin().value;
        prio.removeMin();

        if (!visited[current.to_index()]) {
            visited[current.to_index()] = true;

            std::tie(dist, prev, prio) =
                relax_successors(g, current, dist, prev, prio);
        }
    }

    return {dist, prev};
}
}
