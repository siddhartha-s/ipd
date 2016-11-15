# Imperative Binary Search Trees (this time in C++)

We have seen binary search trees in ISL+λ and, for those trees, we
freely created nodes and relied on the language's runtime support to
handle the memory management of them. That is not how C++ works. So,
we will revising binary search trees and see how they work in a
language where we are tracking the memory being used ourselves and,
accordingly, being much more frugal with it.

In particular, we will be studying a non-persistent binary tree, so
the operations on the tree will modify the tree in place, trying to
re-use as much of the tree structure as we can.

To make this concrete, look at the interface definition here:
[`Bst.h`](src/Bst.h). It shows a container class for binary search
trees. The actual trees are the same as we had in ISL+λ, but we also
have an extra object that encapsulates them and has all of the
operations on them. Lets have a look at the `contains_nottail()`
function. Do you recognize the strategy used there? It looks a little
bit different in C++, but hopefully you recognize it as structural
decomposition. Unfortunately, that is not idiomatic C++, because
evaluation in C++ doesn't follow the rule of substitution for function
calls. Instead, it uses a little bit of stack space for each function
call. In contrast, a `while` loop does not use that space, so the
`contains()` function is idiomatic C++ and actually behaves like the
ISL+λ code.

Okay, lets try to write the insert function. Unlike the insert
function in ISL+λ, this one will find a spot in the tree to update and
then will change the tree, adding a node into that spot. Here is a way
to think about this, in pictures:

```
+-------+
| Bst   |
+-------+      +-------+------+------+-------+
| root_ | ---> | node_ |  12  | left | right |
+-------+      +-------+------+------+-------+
                                 |       |
   +-----------------------------+       |
  \|/                                    |
+-------+------+------+-------+          |
| node_ |  10  | left | right |          |
+-------+------+------+-------+          |
                                         |
                       +-----------------+
                      \|/
                    +-------+------+------+-------+
                    | node_ | 14   | left | right |
                    +-------+------+------+-------+
```

If we wanted to insert 11 into the tree, what would we want to update?
How about 13? How about 15?

So, in all cases, we're going to way to have a pointer to one of those
left or right fields? .... almost. What if the tree was empty, like
this:

```
+---------+
| Bst     |
+---------+
| nullptr | 
+---------+
```

In that case, we'd want a pointer to the `root_` field of the node
itself.

Is there a single type we can use to talk about all of those different
possibilities? Yes: `ptr_*`. That is, the place in memory where a
`ptr_` is. Or, if you want to confuse yourself, a pointer to a pointer
to a node.

Lets try to design the insertion function. We know our data. Next
up: examples as tests (see [`bst_test.cpp`](test/bst_test.cpp)) and then
lets write the function (see [`Bst.h`](src/Bst.h)).

After that, lets write the `delete` function. There are multiple
different ways to go about writing `delete`; we want an approach that
minimizes the (imperative) changes to the tree. One way to do that is 

# Huffman Codes

A huffman code is a way to encode some data to make it take less
space. The basic idea is to compute the frequencies of each byte that
appears in the message and then use that to create codewords for each
character, such that the codewords for more frequent characters are
shorter than the ones for the less frequent characters.

To create the codewords, we first calculate the frequencies of the
bytes that appear in the input. Lets consider the example input:

```
axyzzyzzzz
```

We first calculate the freqeuencies of each different byte:

-  97(a) : 1
-  120 (x): 1
-  121 (y): 2
-  122 (z): 6

and then we think of these as the leaf nodes in a tree that we're
going to build up from the leaves.

So we start with three disconnected leaf nodes:

```
+-----+  +-----+  +-----+  +-----+
| a:1 |  | x:1 |  | y:2 |  | z:6 |
+-----+  +-----+  +-----+  +-----+
```

Next we find the two nodes that have the lowest frequencies, in this
case `a` and `x` and we join them together to make an interior node,
with a frequency that is the sum of the two frequencies:

```
     +-----+
     |  2  |
     +-----+
     /     \
+-----+  +-----+  +-----+  +-----+
| a:1 |  | x:1 |  | y:2 |  | z:6 |
+-----+  +-----+  +-----+  +-----+
```

Since we have not connected everything in the tree yet, we repeat this
process, finding the two with the lowest frequencies and joining them
together:

```
           +-----+
           |  4  |
           +-----+
           /     \ 
     +-----+      \
     |  2  |       \
     +-----+        \
     /     \         \
+-----+  +-----+  +-----+  +-----+
| a:1 |  | x:1 |  | y:2 |  | z:6 |
+-----+  +-----+  +-----+  +-----+
```

and then one more and we have the complete tree:


```
                +-----+
                |  7  |
                +-----+
                /     \
           +-----+     \
           |  4  |      \
           +-----+       \
           /     \        \
     +-----+      \        \
     |  2  |       \        \
     +-----+        \        \
     /     \         \        \
+-----+  +-----+  +-----+  +-----+
| a:1 |  | x:1 |  | y:2 |  | z:6 |
+-----+  +-----+  +-----+  +-----+
```

Using that tree we can read off the code words for each byte by
walking down the tree from the root to each leaf, using a `0` when we
go left and a `1` when we go right. Doing that, we get these encoded
versions of the input bytes:

- `a` -> `000`
- `x` -> `001`
- `y` -> `01`
- `z` -> `1`

and this is the encoded message:

```
a   x   y  z z y  z z z z
000 001 01 1 1 01 1 1 1 1
```

or, in (unsigned) bytes:
```
00000101 11011111
5        223
```

Note we kept adding leaves directly to the top of the tree in this
process, but that doesn't always have to happen. For example, if the
frequences in the original input:

```
+-----+  +-----+  +-----+  +-----+
| a:2 |  | x:2 |  | y:2 |  | z:2 |
+-----+  +-----+  +-----+  +-----+
```

we get a tree that's balanced like this:

```

            +--------+
            |    8   |
            +--------+
            /        \
           /          \
          /            \
         /              \
        /                \
     +----+            +----+
     | 4  |            |  4 |
     +----+            +----+
     /    \            /    \
    /      \          /      \
   /        \        /        \
+-----+  +-----+  +-----+  +-----+
| a:2 |  | x:2 |  | y:2 |  | z:2 |
+-----+  +-----+  +-----+  +-----+
```

And, in general, you can get all different kinds of trees depending on
the frequencies.

When you are doing the assignment, some other things to think about:

- Each file will have different frequences and thus a different
  tree. Accordingly, the file has to save the tree (or some
  information that is equivalent to the tree). How should that be
  stored?

- When you are writing out the file, the encoding may not be some
  number of bits that is not an even multiple of 8, which means you
  cannot rely on the information stored in the filesystem to know how
  long the encoded file is.


# Imperative Binomial Heaps (this time in C++)
