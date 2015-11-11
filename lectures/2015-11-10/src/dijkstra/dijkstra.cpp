// Dijkstra's algorithm for single-source shortest path.

#include "graph/graph.hpp"
#include "binheap/key_val.hpp"
#include "binheap/binheap.hpp"

#include <vector>

namespace dijkstra {

template <typename NodeInfo>
class sssp_container
{
    using gr     = graph::graph<NodeInfo>;
    using node   = typename gr::node;
    using weight = typename gr::weight;

    void sssp(gr g, node start)
    {
        size_t size = g.size();

        std::vector<weight> dist(size, gr::INFINITY);
        std::vector<node> prev(size, g.not_a_node());
        std::vector<bool> seen(size, false);
        binheap::binheap<binheap::pair<weight, node>> prio;

        dist[g.to_index(start)] = 0;
        prio.add(start);

        while (! prio.isEmpty()) {
            auto w_n = prio.getMin();
            auto old_dist = w_n.key;
            auto current  = w_n.value;

            prio.removeMin();

            if (! seen[g.to_index(current)]) {
                seen[g.to_index(current)] = true;

                for (auto succ : g.get_successors(current)) {
                    weight new_dist =
                        dist[g.to_index(current)] + g.get_weight(current, succ);
                    if (new_dist < old_dist) {
                        dist[g.to_index(succ)] = new_dist;
                        prev[g.to_index(succ)] = current;
                        prio.add({ new_dist, succ });
                    }
                }
            }
        }
    }
};

}
