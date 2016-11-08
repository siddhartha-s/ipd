#include "Heap.h"
#include "WU_graph.h"

#include <UnitTest++/UnitTest++.h>

using namespace ipd;

TEST(Insert5Remove5)
{
    Heap<int> heap;
    heap.insert(4);
    heap.insert(6);
    heap.insert(0);
    heap.insert(8);
    heap.insert(2);

    CHECK_EQUAL(5, heap.size());

    CHECK_EQUAL(0, heap.remove_min());
    CHECK_EQUAL(2, heap.remove_min());
    CHECK_EQUAL(4, heap.remove_min());
    CHECK_EQUAL(6, heap.remove_min());
    CHECK_EQUAL(8, heap.remove_min());

    CHECK(heap.empty());
}

TEST(InsertRemove)
{
    Heap<int> h;

    h.insert(8);
    h.insert(7);
    CHECK_EQUAL(7, h.peek_min());
    CHECK_EQUAL(7, h.remove_min());
    h.insert(2);
    h.insert(14);
    CHECK_EQUAL(2, h.remove_min());
    h.insert(20);
    h.insert(1);
    CHECK_EQUAL(1, h.remove_min());
    CHECK_EQUAL(8, h.remove_min());
    CHECK_EQUAL(14, h.remove_min());
    CHECK_EQUAL(20, h.remove_min());
}

struct known_distance
{
    WU_graph::vertex v;
    WU_graph::weight w;
};

bool operator<(const known_distance& kd1, const known_distance& kd2)
{
    return kd1.w < kd2.w;
}

namespace {

bool relaxG(const WU_graph& graph, SSSP_result& sssp,
            WU_graph::vertex v, WU_graph::vertex u)
{
    WU_graph::weight old_dist = sssp.dist[u];
    WU_graph::weight new_dist = sssp.dist[v] + graph.get_edge(v, u);

    if (new_dist < old_dist) {
        sssp.dist[u] = new_dist;
        sssp.pred[u] = v;
        return true;
    } else return false;
}

SSSP_result dijkstraG(const WU_graph& graph,
                      WU_graph::vertex start)
{
    size_t size = graph.size();
    SSSP_result result(size);
    std::vector<bool> visited(size, false);
    Heap<known_distance> heap;

    result.pred[start] = start;
    result.dist[start] = 0;
    heap.insert({start, 0});

    while (!heap.empty()) {
        WU_graph::vertex v = heap.remove_min().v;
        if (visited[v]) continue;

        visited[v] = true;

        for (WU_graph::vertex u : graph.get_neighbors(v)) {
            if (relaxG(graph, result, v, u)) {
                heap.insert({u, graph.get_edge(v, u)});
            }
        }
    }

    return result;
}

}

TEST(Dijkstra)
{
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

    SSSP_result result = dijkstraG(g, 1);

    CHECK_EQUAL(0, result.dist[1]);
    CHECK_EQUAL(1, result.pred[1]);
    CHECK_EQUAL(7, result.dist[2]);
    CHECK_EQUAL(1, result.pred[2]);
    CHECK_EQUAL(20, result.dist[5]);
    CHECK_EQUAL(6, result.pred[5]);
    CHECK_EQUAL(3, result.pred[6]);
    CHECK_EQUAL(1, result.pred[3]);
}

