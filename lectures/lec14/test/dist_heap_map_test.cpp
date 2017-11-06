#include "Dist_heap_map.h"
#include "WU_graph.h"

#include <UnitTest++/UnitTest++.h>

using namespace ipd;

TEST(Update5Remove5)
{
    Dist_heap_map h;
    h.update({0, 4});
    h.update({1, 6});
    h.update({2, 0});
    h.update({3, 8});
    h.update({4, 2});

    CHECK_EQUAL(0, h.remove_min().w);
    CHECK_EQUAL(2, h.remove_min().w);
    CHECK_EQUAL(4, h.remove_min().w);
    CHECK_EQUAL(6, h.remove_min().w);
    CHECK_EQUAL(8, h.remove_min().w);
}

TEST(DecreaseKey)
{
    Dist_heap_map h;
    h.update({0, 4});
    h.update({1, 6});
    h.update({2, 0});
    h.update({3, 8});
    h.update({4, 2});

    h.update({3, 1});

    CHECK_EQUAL(0, h.remove_min().w);
    CHECK_EQUAL(1, h.remove_min().w);
    CHECK_EQUAL(2, h.remove_min().w);
    CHECK_EQUAL(4, h.remove_min().w);
    CHECK_EQUAL(6, h.remove_min().w);
}

bool relaxH(const WU_graph& graph, SSSP_result& sssp,
            WU_graph::vertex v, WU_graph::vertex u)
{
    WU_graph::weight old_dist = sssp.dist[u];
    WU_graph::weight new_dist = sssp.dist[v] + graph.get_edge(v, u);

    if (new_dist < old_dist) {
        sssp.dist[u] = new_dist;
        sssp.pred[u] = v;
        return true;
    } else {
        return false;
    }
}

SSSP_result dijkstraH(const WU_graph& graph, WU_graph::vertex start)
{
    size_t size = graph.size();
    SSSP_result result(size);
    std::vector<bool> visited(size, false);
    Dist_heap_map heap;

    result.pred[start] = start;
    result.dist[start] = 0;
    heap.update({start, 0});

    while (!heap.empty()) {
        WU_graph::vertex v = heap.remove_min().v;
        visited[v] = true;

        for (WU_graph::vertex u : graph.get_neighbors(v)) {
            if (!visited[u]) {
                if (relaxH(graph, result, v, u)) {
                    heap.update({u, graph.get_edge(v, u)});
                }
            }
        }
    }

    return result;
}

TEST(Dijkstra) {
    WU_graph g(7);
    g.add_edge(1, 2, 7);
    g.add_edge(1, 3, 9);
    g.add_edge(1, 6, 14);
    g.add_edge(2, 3, 10);
    g.add_edge(2, 4, 15);
    g.add_edge(3, 4, 11);
    g.add_edge(3, 6, 2);
    g.add_edge(4, 5, 6);
    g.add_edge(5, 6, 9);

    auto result = dijkstraH(g, 1);

    CHECK_EQUAL(0, result.dist[1]);
    CHECK_EQUAL(1, result.pred[1]);
    CHECK_EQUAL(7, result.dist[2]);
    CHECK_EQUAL(1, result.pred[2]);
    CHECK_EQUAL(20, result.dist[5]);
    CHECK_EQUAL(6, result.pred[5]);
}
