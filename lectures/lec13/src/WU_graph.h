#pragma once

#include <limits>
#include <vector>

namespace ipd {

// A class representing a weighted, undirected graph.
class WU_graph
{
public:
    // Vertices are natural numbers from 0 to |V| - 1:
    using vertex    = size_t;
    // Weights are doubles:
    using weight    = double;

    // NO_EDGE == +inf.0, representing the absence of an edge:
    static const weight NO_EDGE;

    // Constructs a new graph with the given number of vertices.
    WU_graph(size_t);

    // Adds an edge with the given weight.
    void add_edge(vertex, vertex, double);

    // Returns the number of vertices in this graph.
    size_t size() const;

    // Gets a vector of all vertices connected to the given vertex by finite
    // edges.
    std::vector<vertex> get_neighbors(vertex) const;

    // Gets the weight of the edge between two vertices, which may be `NO_EDGE`.
    weight get_edge(vertex, vertex) const;

private:
    std::vector<std::vector<weight>> weights_;

    void bounds_check_(vertex) const;

    friend bool operator==(const WU_graph&, const WU_graph&);
};

// Compares two graphs for equality by comparing all edges and weights.
bool operator==(const WU_graph&, const WU_graph&);

// A triple representing a single weighted edge. Used in return value of
// `get_all_edges` (below).
struct WU_edge {
    using vertex = WU_graph::vertex;
    using weight = WU_graph::weight;

    WU_edge(vertex u, vertex v, weight w) : u{u}, v{v}, w{w} {}

    vertex u, v;
    weight w;
};

// Gets a vector of all edges in the graph.
std::vector<WU_edge> get_all_edges(const WU_graph&);

// Holds the result of computing SSSP.
struct SSSP_result {
    // Initializes an SSSP result for a graph of the given size.
    SSSP_result(size_t);

    // Gives the predecessor along the shortest path to each vertex, or an
    // invalid value if that vertex is unreachable.
    std::vector<WU_graph::vertex> pred;
    // Gives the distance from the start node to each vertex, or
    // WU_graph::NO_EDGE if unreachable.
    std::vector<WU_graph::weight> dist;
};

// Computes SSSP by Bellman-Ford algorithm.
SSSP_result bellman_ford(const WU_graph&, WU_graph::vertex start);

// Computes SSSP by Dijkstra's algorithm.
SSSP_result dijkstra(const WU_graph&, WU_graph::vertex start);

}