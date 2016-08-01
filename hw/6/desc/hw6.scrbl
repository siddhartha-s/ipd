#lang scribble/base

@(require scribble/core
          scribble/manual
          scribble/latex-properties
          scribble/html-properties
          (only-in pict scale))

@title[#:style (style
                #f
                (list
                 ;(document-version "6.2.1")
                 (latex-defaults
                  '(collects #"scribble" #"manual-prefix.tex")
                  '(collects #"scribble" #"manual-style.tex")
                  '())
                 (html-defaults
                  '(collects #"scribble" #"scribble-prefix.html")
                  '(collects #"scribble" #"manual-style.css")
                  '((collects #"scribble" #"manual-fonts.css")))
                 (css-style-addition '(collects #"scribble" #"manual-racket.css"))
                 (js-style-addition '(collects #"scribble" #"manual-racket.js"))))
       ]{Homework 6}
@bold{EECS 495 Intensive Program Design}

@bold{Due date:} Tuesday, November 17, 11:59 PM @;{TODO - udpate - when?}

@bold{Pair program this assignment with your partner.}
Remember, that means that all code should be written while
both of you are sitting together.

@bold{Grading.} You will be graded in codewalks the Wednesday
after this assignment is due. Again, your grade will
be based principally on your application of the design
recipe and your explanations in your code walk.

@bold{Setup.} Download this archive:
@elem[#:style (style "dl" (list (link-resource "hw6.tar.gz")))]{hw6.tar.gz}.
It contains a @link["https://cmake.org/"]{CMake} project
that you can use to generate a projects for a number of
systems. Through a Unix shell, you can run the tests easily by
running @tt{make test} in the project root directory.
(If there are failing tests, @tt{make report} will
give you more details.)

@bold{Submitting.} Return an archive of your completed project to
@tt{burke.fetscher@"@"eecs.northwestern.edu}, with the
subject:
@centered{@tt{EECS 495 - HOMEWORK 6 <your net id> <your partner's net id>}}

@section{Disjoint Sets of Integers}

In this exercise you will design a data structue representing disjoint
sets of nonnegative integers.
The operations required are:
@itemlist[
 @item{Create an initial set of sets. (We'll use a constructor for this.)}
 @item{@tt{find}, which takes an integer as its argument,
  and returns a representative member of the set it belongs to.}
 @item{@tt{union}, which takes two integers as its arguments and
        combines the sets they belong to.}]
It's easiest to see how these work with an example.
Suppose you start by constructing four sets, each containing an integer from
the range @italic{[0, 4)}:
@centered{@italic{{0} {1} {2} {3}}}
At this point, @tt{find(@italic{n}) == @italic{n}}, for any @italic{n}
in @italic{[0, 4)}, since each set only has one member.

After the operation @tt{union(1,3)}, your sets will look like:
@centered{@italic{{0} {1,3} {2}}}
At this point, you don't know if @tt{find(1)}
is @tt{1} or @tt{3}, but it is a requirement that there is only
one representative for each set, so @tt{find(1) == find(3)}. In fact,
this is how we can check to see if two numbers belong to the same set.

After the operation @tt{union(1,2)}, the sets will look like:
@centered{@italic{{0} {1,3,2}}}
And after @tt{union(3,0)}, there will be a single set and for any
@italic{m,n ∈ [0, 4)}, @tt{find(@italic{m}) == find(@italic{n})}.

You'll implement these sets as a class with a constructor creating
a range of singleton sets, as above, and two public functions:
@tt{do_union} and @tt{find}. A skeleton for the class is in
@tt{src/union_find} in the provided archive. The constructor should
take a size argument @italic{n} and create a representation of the singleton
sets: @italic{{0} {1} ... {n-1}}.

Implement your sets using a vector @tt{v}, such that each element in the vector
is also an index into the vector. If the element at position @tt{n} is @tt{n}, then @tt{n}
is a representative, otherwise we can find @tt{n}'s representative by
looking at the element at position @tt{v[n]}. So if the vector looks like:
@tt{[0,1,2,3]}, then that is a representation of the singleton sets constructed
initially above. And the vector @tt{[0,1,2,1]} is one possible representation
of the situation after @tt{union(1,3)}, and in this case @tt{find(3) == 1} and
@tt{find(1) == 1}. As in the binary heap implementation from class, we are really
tracking an implicit tree in a vector: the roots are the representative elements,
and the pointers go upward.

To make this structure efficient, you need to do two things:
@itemlist[
 @item{@bold{path compression} - on every recursive call to @tt{find},
  update the element in the vector at the argument index to be the representative,
  so that after a call to @tt{find}, every element you have seen now ``points'' directly
  to the representative.}
 @item{@bold{union by rank} - keep track of the rank (depth of each tree, or for each element, the
 maximum number of times @tt{find} would have to recur to get to that element), and when performing
 a union on two elements, always make the smaller tree the child of the larger.}]

To help get started, some tests are provided in @tt{tests/union_find.cpp}.

With the optimizations you've implemented, this algorithm is
@italic{O(α(n))}, where @italic{α} is the inverse Ackermann function,
an extremely slow-growing function, so it's very efficient.

@section{Minimum Spanning Trees}

Now use your implementation of integer sets from the first exercise
and the graph representation from class to implement an algorithm
for finding the @italic{minimum spanning tree} for a graph, the tree of
minimum total weight that includes every node in the original graph.
We'll use @italic{Kruskal's algorithm} (@italic{O(E log E)}), although
there are other comparably efficient approaches.

It works as follows:
@itemlist[
 @item{Initialize a disjoint sets structure containing singleton sets for each node.}
 @item{Sort the edges in order of increasing weight. Use @tt{std::sort} for this.
 You'll need an auxiliary structure to represent the edges.}
 @item{Go through the edges in order, and if an edge has the property that its
  source and destination nodes are in different sets, then add it to a list of
  edges for the MST, and perform a union of the sets containing the two nodes.
  (If the source and destination are in the same set, then the nodes are already
  connected, so just discard this edge and go on to the next one.)}]

Once you've gone through all the edges, the edges you've picked will constitute
an MST. (This might not the case if the original graph was not connected, but you
may assume the original graph is connected as a precondition to this function.
Bonus points if you don't and handle it correctly, though.)

See the skeleton function @tt{mstk} at the end of @tt{src/graph/graph.hpp}. You need
to complete the implementation of this function. There are some tests in
@tt{test/mst.cpp} to get you started.