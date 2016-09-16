# Binary heap and generics

## Analyzing Dijkstra’s algorithm

Last time we implemented Dijkstra’s SSSP algorithm. In pseudocode, it goes 
like this:

1.  For all vertices v:
      - dist[v] := infinity
      - pred[v] := undef
      - visited[v] := false
      
2.  For the starting vertex s,
      - dist[s] := 0
      
3.  Find the minimum distance unvisited vertex v; if there is no such vertex,
    terminate.
    
4.  Mark v as visited. Relax each outgoing edge of v.

5.  Go to 3.

What is the time complexity of this algorithm?

Well, step 1 takes O(V), proportional to the number of vertices. Step 2 is 
constant time. Step 3 as we wrote it scans every vertex, making it O(V), but 
let’s suppose we can do better, and for now call that step O(get_min(V)); 
repeated at most once per vertex, it comes to O(V get_min(V)).
Step 4 happens once (or maybe twice) for each edge, for O(E). Thus, our total
time is O(E + V get_min(V)). Using the linear scan, that means that 
Dijkstra's algorithm takes O(E + V^2) = O(V^2). But do we know a way that we 
can do get_min faster?

(We saw binomial heaps, which implement the Priority Queue ADT.)

## The Binary Heap

Another kind of heap (priority queue implementation) is the bin*ary* heap. 
Unlike the bin*omial* heap, a binary heap is a binary tree. Not only that, 
but it’s a *complete* binary tree, which means that every level of the tree 
is completely filled, except possibly the last level, which is filled from 
the left. That is, nodes are only added and removed at the end of the
level-order traversal. This will be important in a bit.

The binary heap has the same heap condition as the binomial heap, namely, 
that every node’s value is less than its children’s values (and by 
transitivity, their children’s).

To add an element to the heap, we add a node
containing the new element to the end of the level-order traversal—that is, 
to the right of the previous “last” node. Then we compare its value to its 
parent’s value, and if the new node’s value is less than the parent’s, we 
swap their values. Then compare the parent to the grandparent, the 
grandparent to the great grandparent, and so on, stopping when we find a node
whose parent’s value is less than the node. This procedure, called bubbling 
up, restores the heap invariant. Can you see why?

Finding the minimum element is easy—it’s always the root. To remove it, we 
swap the root’s value with the last (per level-order traversal) node’s value 
and then remove the last node. This means that the former last value is now 
at the root, and it may be greater than one of its children! So we check its 
children, finding the smaller of the two, and swap it with that one. Then 
proceed down the tree, swapping with the smaller of the children, until 
either the node is less than both children, or there are no children to swap 
with. This procedure, called trickling down, also restores the heap invariant.
Why?

### Why a complete tree?

Here‘s the cool thing about binary heaps: A complete tree can be represented 
as a stack, or in other words, a vector that grows and shrinks only at the end.
We store the values in level traversal order, so the root comes at index 0, 
its left child at index 1, its right child at 2, 1’s left child at 3, 1’s 
right child at 4, and so on. Can you write the function that, given an index
into the vector, returns the parent’s index? How about the children?

### Implementation

The goal of the heap in Dijkstra’s algorithm is to keep vertices ordered by 
distance, so we need a heap that stores pairs of each vertex with its best 
known distance, and orders them by the distance. The interface for a 
best-known distance heap appears in `src/Distance_heap.h`, and the 
implementation appears in `src/Distance_heap.cpp`. Note that each is 
represented as a vector as discussed in the previous section. The vector is a
private data member, because we want clients to the heap via its ADT 
operations, not by manipulating the vector directly.

Tests for the implementation appear in `test/dist_heap_test.cpp`, including 
an implementation of Dijkstra’s algorithm that uses the `Distance_heap` to 
visit vertices in the right order.

## Generics

The binary heap we just made stores pairs of a vertex and a distance,
ordered by 
distance. But nothing about the generic description of it requires those 
particular pieces of information. We could make a binary heap of anything, 
provided we know how to order it. That is, instead of having a 
`Distance_heap`, we could have a `Heap<X>` for different `X`s, such as 
`Heap<known_distance>` for Dijkstra. (This is like `[Heap-of X]` back in ISL.)

We do this by making the class declaration into a template:

```
template <typename Element>
class Heap
{
    void insert(const Element&);
    ...
```

Read `template` as “for all”: For all types `Element`, class `Heap` is has 
the following members: an `insert` function taking a contant reference to an 
`Element`…

When you use `Heap`, like `Heap<int>` or `Heap<double>`, then `Element` means
 `int` or `double`. (It’s substitution.) See the rest of the heap template 
declaration in `src/Heap.h`. You may notice that there’s much more code in 
that file than usual—in fact, the whole implementation is in the header, and 
there’s no .cpp file. Why? One of the rules of templates is that all 
templated code, which includes all the member functions of the class, *must 
appear in a header*.

You should also notice that each member declared outside the class has a 
`template <typename Element>` line in front of it, and that’s because each 
member works for any `Element` type, and we need to say so. Take a look at 
where the member functions compare elements to decide whether to swap them, 
and you will see that they are no longer assuming the particular 
`known_distance` struct, but rather comparing the elements directly.

You should also notice that the helpers for converting tree movements to 
array indices (`parent`, `left_child`) are in a sub-namespace, `heap_helpers`.
This is so they don’t pollute the main `ipd` namespace, since they don’t have
meaning outside that file. And note that they are marked `inline`, which is 
required for non-template functions defined in headers. (If you want to try 
to understand why, look up C++’s “One Definition Rule.”)

`test/heap_test.cpp` contains a test of a `Heap<int>` and a version of 
Dijkstra’s algorithm using `Heap<known_distance>`. Note that
`operator<(const known_distance&, const known_distance&)` has to be 
overloaded to tell the heap how to order things.
