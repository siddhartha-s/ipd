#lang dssl2

import cons

# Graph Search
# ------------

# A week ago we were introduced to a graph representation, and we wrote
# a function to determine when a graph has a path between two given
# nodes. The function we wrote was very inefficient—exponential time,
# actually—because it explored every path through the graph. (Remember
# the example?) We can do better using less naive graph search
# algorithms: depth-first search and breadth-first search. Let’s look
# at DFS first.

# A Graph is:
#   graph {
#     nodes: Nat,
#     succs: Nat[< nodes] -> [ListOf Nat[< nodes]]
#   }
# the `nodes` field tells us the number of nodes in the graph;
# the `succs` field gives us a function from each node to the list
# of its successors.
defstruct graph(nodes: nat?, succs: FunC(nat?, list?))

# my_succs_function : nat? -> ListOf[nat?]
# A hard-coded successors function:
#
# Strategy: decision tree
def my_succs_function(node: nat?) -> list?:
    if   node == 0: cons(1, cons(2, cons(3, nil())))
    elif node == 1: cons(3, cons(0, cons(4, nil())))
    elif node == 2: cons(4, cons(0, nil()))
    elif node == 3: cons(4, cons(1, cons(2, nil())))
    elif node == 4: cons(5, cons(4, nil()))
    elif node == 5: nil()
    elif node == 6: cons(3, cons(4, nil()))
    else: error("Unknown node: ~e", node)

# A_GRAPH0 : graph?
# A graph built using the hard-coded successors function
let A_GRAPH0 = graph(7, my_succs_function)

test 'hard-coded graph is correct':
    assert_eq A_GRAPH0.nodes, 7
    assert_eq A_GRAPH0.succs(2), cons(4, cons(0, nil()))

# mk_adj_vec_graph : VectorOf[VectorOf[nat?]] -> graph?
# Builds a graph from a vector of adjacency vectors.
#
# Strategy: function composition
def mk_adj_vec_graph(vecs: vec?) -> graph?:
    def succs(v): cons_from_vec(vecs[v])
    graph(len(vecs), succs)

let A_GRAPH = mk_adj_vec_graph([
    [1, 2, 3],
    [3, 0, 4],
    [4, 0],
    [4, 1, 2],
    [5, 4],
    [],
    [3, 4]
])

# A SearchTree is VectorOf[OrC[nat?, bool?]]
# where for each node n, the nth element is the parent node of node n;
# the value for the root is True, and the value for unreachable nodes is
# False.

# dfs : Graph Nat -> SearchTree
# Searches `a_graph` starting at `start_node`, returning the search
# tree.

# Strategy: generative recursion. Termination: `visit` can only recur
# a finite number of times, because each time it recurs it removes a
# node from the unvisited set, which can happen only a finite number of
# times.
def dfs(a_graph: graph?, start_node: nat?) -> vec?:
    let result = [ False; a_graph.nodes ]
    def visit(node: nat?) -> VoidC:
        def each(succ: nat?) -> VoidC:
            if !result[succ]:
                result[succ] = node
                visit(succ)
        foreach_cons(each, a_graph.succs(node))
    result[start_node] = True
    visit(start_node)
    result

test 'dfs tests':
    let T = True; let F = False
    assert_eq dfs(A_GRAPH, 0), [T, 0, 3, 1, 3, 4, F]
    assert_eq dfs(A_GRAPH, 1), [2, T, 3, 1, 3, 4, F]
    assert_eq dfs(A_GRAPH, 5), [F, F, F, F, F, T, F]
    assert_eq dfs(A_GRAPH, 6), [1, 3, 0, 6, 3, 4, T]

# Now we can write a much more efficient route-exists?:

# route-exists? : graph? nat? nat? -> bool?
# Determines whether the graph contains a path from `start` to `end`.
#
# Strategy: function composition
def route_exists?(a_graph: graph?, start: nat?, end: nat?) -> bool?:
    !!dfs(a_graph, start)[end]

test 'route_exists? tests':
    assert route_exists?(A_GRAPH, 0, 4)
    assert !route_exists?(A_GRAPH, 0, 6)
    assert route_exists?(A_GRAPH, 3, 4)
    assert !route_exists?(A_GRAPH, 4, 3)
    assert route_exists?(A_GRAPH, 5, 5)


# DFS uses the stack-like structure of evaluation to remember which
# node to search next. We can instead make the stack of nodes to search
# explicit, and then we can generalize that to get other graph search
# algorithms.
#
# In particular, we can generalize graph search as follows. Given a
# graph, start with an empty to-do list and empty search tree. Add the
# given starting node to the to-do list. Then repeat so long as the
# to-do list is non-empty: Remove a node n (any node) from the to-do
# list and examine each of its successors. If successor s has not been
# seen according to the search tree, then record n as s's predecessor
# in the search tree and add s to the to-do list.
#
# To implement this algorithm we need a data structure to serve as the
# to-do list. We can partially specify the requirements as an ADT:

# A ContainerOf[X] is Container {
#    empty?  : -> bool?
#    add!    : X -> VoidC
#    remove! : -> X
# }
defstruct Container(empty?, add!, remove!)

# Different container implementations will remove nodes in different
# orders, yielding different kinds of graph searches.

# We can write our search in a generic way, where we don’t specify
# ahead of time what kind of container to use for the to-do list. To do
# this, we pass in factory, which contains a function that creates a new,
# empty container.

# A ContainerFactoryC is a [-> ContainerOf[X]]
let ContainerFactoryC = FunC(Container?)

# container-example : ContainerFactoryC ListOf[X] -> ListOf[X]
# Uses `factory` to create an empty container, to which it adds the elements
# of `elements` in order; then removes all the elements and returns them
# in a list in the order removed.
def container_example(factory: ContainerFactoryC, elements: list?) -> list?:
    let result = nil()
    let container = factory()
    while cons?(elements):
        container.add!(elements.car)
        elements = elements.cdr
    while !container.empty?():
        result = cons(container.remove!(), result)
    rev_cons(result)

# And here's a simple implementation of a Container:

def ListStack() -> Container?:
    let list = nil()

    def empty?():
        nil?(list)

    def add!(element):
        list = cons(element, list)

    def remove!():
        if cons?(list):
            let result = list.car
            list = list.cdr
            return result
        else: error('ListStack::remove!: empty')

    Container { empty?, add!, remove! }

let list = cons_from_vec

test 'container_example test with ListStack':
    assert_eq container_example(ListStack, nil()), nil()
    assert_eq container_example(ListStack, list([2])), list([2])
    assert_eq container_example(ListStack, list([2, 3, 4])), list([4, 3, 2])

# Now we can write the search algorithm generically, where it is passed
# what kind of container to use:
#
# generic_search : ContainerFactoryC graph? nat? -> SearchTree
# Performs a graph search from the given start node, using the given
# container implementation to order the search.
#
# Strategy: generative iteration. Termination: each iteration through
# the loop removes a node from `to-do`, and each node is added to `to-do`
# at most once, since it is also marked in `result` as having been visited,
# and thus won’t be visited again.
def generic_search(factory: ContainerFactoryC,
                   a_graph: graph?, start: nat?) -> vec?:
    let result = [ False; a_graph.nodes ]
    let to_do = factory()
    result[start] = True
    to_do.add!(start)
    while !to_do.empty?():
        let node = to_do.remove!()
        def each(succ):
            if !result[succ]:
                result[succ] = node
                to_do.add!(succ)
        foreach_cons(each, a_graph.succs(node))
    result

test 'generic_search A_GRAPH with stack':
    let T = True; let F = False
    assert_eq generic_search(ListStack, A_GRAPH, 0), [T, 0, 0, 0, 3, 4, F]
    assert_eq generic_search(ListStack, A_GRAPH, 5), [F, F, F, F, F, T, F]
    assert_eq generic_search(ListStack, A_GRAPH, 6), [2, 3, 3, 6, 6, 4, T]
# ^ This is doing DFS.


### Another possible container is a queue:

def BankersQueue() -> Container?:
    let front = nil()
    let back = nil()

    def empty?():
        nil?(front) and nil?(back)

    def add!(element):
        back = cons(element, back)

    def remove!():
        if nil?(front):
            if nil?(back):
                error('BankersQueue::remove!: empty')
            while cons?(back):
                front = cons(back.car, front)
                back = back.cdr
        let result = front.car
        front = front.cdr
        result

    Container { empty?, add!, remove! }

test 'container_example test with BankersQueue':
    assert_eq container_example(BankersQueue, nil()), nil()
    assert_eq container_example(BankersQueue, list([2])), list([2])
    assert_eq container_example(BankersQueue, list([2, 3, 4])), list([2, 3, 4])

test 'generic_search A_GRAPH with BankersQueue':
    let T = True; let F = False
    assert_eq generic_search(BankersQueue, A_GRAPH, 0), [T, 0, 0, 0, 1, 4, F]
    assert_eq generic_search(BankersQueue, A_GRAPH, 5), [F, F, F, F, F, T, F]
    assert_eq generic_search(BankersQueue, A_GRAPH, 6), [1, 3, 3, 6, 6, 4, T]
# ^ This is doing BFS (breadth-first search).
