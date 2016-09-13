# IPD Homework 6

  - **Due: Tuesday, November 8 at 11:59 PM**

## Summary

In this assignment you will implement a ranked, path-compressing
union-find, and then you will use it to implement Kruskal’s MST
algorithm.

## Goals

The goals of this assignment are to gain familiarity with union-find and
with graph algorithms, and to practice using and implementing ADTs as
classes.

## Specification

There are two parts to this assignment, union-find and MST.

### Part I: Union-Find

File `src/Union_find.h` contains an interface to the Union-Find ADT as a
C++ class declaration. Your job is 1) to choose a representation and add
the requisite private member variables, and 2) to implement the
union-find operations using your representation.

For the former, you should edit `src/Union_find.h` where it says “`YOUR
CODE HERE`” in order to define your representation.

For the latter, you should implement the constructor and member
functions of the `Union_find` class in `src/Union_find.cpp`. Use the
final version of union-find that we saw in class: lazy union with path
compression, and ranking to prevent trees from becoming imbalanced.

Of course, you need to write tests to ensure that your implementation is
correct. Write them in `test/uf_test.cpp`.

### Part III: Minimum Spanning Tree

For this part, you will use (but not modify) the graph class in
`src/WU_graph.h` and `src.WU_graph.cpp`. In particular, file `src/mst.h`
contains a declaration of a minimum-spanning tree function `kruskal`,
which takes a `WU_graph` (a weighted, undirected graph) and returns a
new `WU_graph` containing only its minimum spanning tree/forest.
In `src/mst.cpp`, you must implement the `kruskal` function by using
Kruskal’s algorithm to compute the MST of the given graph.

## Deliverables

Your deliverables are:

  - The source code of the `Union_find` class and `kruskal` function as
    described above.

  - A plain text file `EVALUATION.md` (which can use
    [Markdown](https://help.github.com/articles/github-flavored-markdown/)
    formatting if you like) in which you describe the design of your
    program. In particular please discuss, briefly:

      - how you tested your programs to ensure correctness and

      - anything that surprised you while doing this assignment.

    When discussing your code, please provide file and line number
    references.

## How to submit

Please submit a ZIP or TAR archive of your whole project. Your project
should build in CLion using the configuration in CMakeLists.txt. Before
creating the archive, be sure to clean your project (*Run* menu /
*Clean*).
