#lang dssl2

import cons

# Problem: find the shortest path between two points in a weighted graph.
#
# It turns out that the reasonable ways to compute this need to compute more
# information than we actually need, in particular, we typically will need to
# compute the shortest path from the starting node to *every* node. This is
# called the Single-Source Shortest Path problem.
#
# To work on this problem, first we need a representation of weighted graphs.
#
# A WEdge is WEdge(nat?, num?, nat?)
defstruct WEdge(src: nat?, weight: num?, dst: nat?)
# Interpretation: an edge exists from `src` to `dst` with weight
# `weight`.

# A WGraph is WGraph(nat?, [nat? -> ListOf[WEdge]])
defstruct WGraph(nodes: nat?, edges: proc?)
# Interpretation: `nodes` gives the number of nodes in the graph, and
# the function `edges` is a function that, given a node, returns a
# list of all departing edges.

## (Note: WGraph can be used to represent a weighted, directed graph, or
## an undirected graph as a special case of that. The algorithms below
## work on both, but our example is an undirected graph.)

# build_wdigraph : nat? VectorOf[Vector[nat?, num?, nat?]] -> WGraph
# Builds a directed graph with `nodes` nodes and the edges specified
# by `edges`.
def build_wdigraph(nodes, edges):
    let adjacencies = [ nil(); nodes ]
    def cons!(edge, v): adjacencies[v] = cons(edge, adjacencies[v])
    for edge in edges:
        cons!(WEdge(edge[0], edge[1], edge[2]), edge[0])
    WGraph(nodes, lambda v: adjacencies[v])
    
# build_wugraph : nat? VectorOf[Vector?[nat?, num?, nat?]] -> WGraph
# Builds an undirected graph with `nodes` nodes and the edges specified
# by `edges`.
def build_wugraph(nodes, edges):
    let rev_edges = [ [edge[2], edge[1], edge[0]] for edge in edges ]
    let all_edges = cons_to_vec(app_cons(cons_from_vec(edges),
                                         cons_from_vec(rev_edges)))
    build_wdigraph(nodes, all_edges)

# Graph from Wikipedia Dijkstra’s algo page
let A_GRAPH = build_wugraph(7, [
      [1,  7, 2],
      [1,  9, 3],
      [1, 14, 6],
      [2, 10, 3],
      [2, 15, 4],
      [3, 11, 4],
      [3,  2, 6],
      [4,  6, 5],
      [5,  9, 6],
])

# SSSP has an interesting structure. Consider this: Suppose the last node
# on the shortest path to v is u. Then the rest of the shortest path to v
# is the same as the shortest path to u. So to track shortest paths, we
# just need to know the path predecessor nodes—the parents in a search tree.
#
# An SSSPResult is sssp(VectorOf[Or[nat?, bool?]] VectorOf[Weight])
defstruct sssp(preds, weights)
# Interpretation: `preds` gives the predecessor on the shortest path to
# each node, and `weights` gives the weight of the path. For unreachable
# nodes `preds` will be false, and for the start node it will be true.
# For unreachable nodes, `weights` will be inf, which is DSSL2’s representation
# of infinity.

# A Weight is one of:
#   - num?
#   - inf

# new_sssp_from : nat? nat? -> sssp?    
# Creates a new SSSPResult for `size` vertices, initializing it for starting
# the search at `start_node`
def new_sssp_from(size: nat?, start_node: nat?) -> sssp?:
    let result = sssp([ False; size ], [ inf; size ])
    result.preds[start_node] = True
    result.weights[start_node] = 0
    result

# The shortest path to any node v must pass through one of its graph
# predecessor nodes u, and its distance will be the sum of shortest path
# length to u and the edge weight from u to v. So to find the shortest
# path, provided we know the shortest paths to all possible u nodes, we
# just have to inspect each possibility and minimize. One way to think of
# this is that we gain knowledge as we search the graph, and in particular
# when we arrive at a node v from a node u, we know that its path is no
# worse than the path to u plus the edge to v. So we can check if we’ve
# found a better path, and update our knowledge if we haven’t.
#
# This update is called relaxation.

# relax : SSSPResult nat? nat? Weight -> Void
# Updates the distance to `v` given the best distance to `u` found so far
# and the weight `weight` of the edge between `u` and `v`.
def relax(result, u, weight, v):
    let old_weight = result.weights[v]
    let new_weight = weight + result.weights[u]
    if new_weight < old_weight:
        result.weights[v] = new_weight
        result.preds[v] = u

# One way we can compute SSSP is to relax all the edges enough times that
# all information propagates. How many times at most do we have to relax
# the edges of a graph?

# bellman_ford : WGraph? nat? -> SSSPResult
# Computes SSSP from the given start node.
# ASSUMPTION: The graph contains no negative cycles.
def bellman_ford(a_graph, start_node):
    let result = new_sssp_from(a_graph.nodes, start_node)
    for i_ in a_graph.nodes:
        for u in a_graph.nodes:
            foreach_cons(lambda edge: relax(result, u, edge.weight, edge.dst),
                         a_graph.edges(u))
    result

test 'bellman_ford tests':
    let F = False; let T = True
    assert_eq bellman_ford(A_GRAPH, 1), sssp([F, T, 1, 1, 3, 6, 3],
                                             [inf, 0, 7, 9, 20, 20, 11])
    assert_eq bellman_ford(A_GRAPH, 3), sssp([F, 3, 3, T, 3, 6, 3],
                                             [inf, 9, 10, 0, 11, 11, 2])


# However, if we are clever (and if there are no negative edges, which will
# mess this up), we can do it faster. In particular, we can relax each edge
# only once, provided we do it in the right order. What’s the right order?
#
# (We want to see the nodes in order from closest to the start to farthest.
# That way, every time we look at an edge, we know that the source of the edge
# already has its shortest path found.)

# dijkstra : WGraph? nat? -> SSSPResult
# Computes SSSP from the given start node.
# ASSUMPTION: The graph contains no negative edges.
def dijkstra(a_graph: WGraph?, start_node: nat?) -> sssp?:
    let visited = [ False; a_graph.nodes ]
    let result = new_sssp_from(a_graph.nodes, start_node)

    def find_nearest_unvisited():
        let best_so_far = False
        for v in a_graph.nodes:
            if !visited[v] and (!best_so_far or
                    result.weights[v] < result.weights[best_so_far]):
                best_so_far = v
        best_so_far

    let u = find_nearest_unvisited()
    while u:
        foreach_cons(lambda edge: relax(result, u, edge.weight, edge.dst),
                     a_graph.edges(u))
        visited[u] = True
        u = find_nearest_unvisited()

    result
    
test 'dijkstra tests':
    let F = False; let T = True
    assert_eq dijkstra(A_GRAPH, 1), sssp([F, T, 1, 1, 3, 6, 3],
                                         [inf, 0, 7, 9, 20, 20, 11])
    assert_eq dijkstra(A_GRAPH, 3), sssp([F, 3, 3, T, 3, 6, 3],
                                         [inf, 9, 10, 0, 11, 11, 2])

## What’s the time complexity of our implementation of Dijkstra’s algorithm?
## How might we improve it?
