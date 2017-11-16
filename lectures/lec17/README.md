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
operations on them. Lets have a look at the `contains_nottail()` and
`contains_ptr()` member functions. Do you recognize the strategies? It
looks a little bit different in C++, but hopefully you recognize
`contains_ptr()`'s structural decomposition and the template for
`node_` / `ptr_`. Unfortunately, that is not idiomatic C++, because
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
minimizes the (imperative) changes to the tree.

# Huffman Codes

When we want to store data, we need a way to encode it and decode
it. There are different ways to talk about what form encoded data
takes, for today we will consider a sequence of bits (booleans, 1/0)
as the only form of encoded data.

A standard way to encode an English text is to use ASCII. It takes 8
bits for each letter (using only 7 of them, the last bit is always 0)
and then concatenates them together. For example, to encode the text:

```
axyzzyzzzz
```

we'd use the mapping:

```
a -> 97  = 01100001 (in binary)
x -> 120 = 01111000
y -> 121 = 01111001
z -> 122 = 01111010
```

and this is the encoded text:

```
01100001011110000111100101111010011110100111100101111010011110100111101001111010
```

It takes 80 bits to encode it using ASCII.

This kind of code is a called a "block code" because each letter of
the input (called "symbols" in coding terminology) takes the same
number of bits in the output (8 in this case).

Another approach is to pick different length sequences of bits for
each different symbol in the input. Once you do that, however, you
have to be careful about concatenating the codes to make a
message. For example, lets say we used these codes:

```
a -> 0
x -> 1
y -> 01
z -> 10
```

And then we encoded our message:

```
axy z z y z z z z
010110100110101010
```

That same sequence of bits could also have been this message:

```
y axz z y z z z z
010110100110101010
```

or a whole bunch of other messages.

One way to avoid this problem is to use something called "prefix
codes". In a prefix code, no encoded symbol has a code that is a
prefix of any other symbol. In the codes just above, we can see that
`a`'s code is a prefix of `y`'s code and `x`'s is a prefix of `z`s.

If we change the code to this:

```
a -> 00
x -> 11
y -> 01
z -> 10
```

then we have that no code is a prefix of the other and, as long as we
start at the beginning of the stream, there is only one way to decode
the encoded text:

```
a x y z z y z z z z
00110110100110101010
```

One way people have thought to organize and think about prefix codes
it to put them into a tree. That is, if you have a binary tree whose
leaf nodes are are the symbols, then you can use the path from the
root to the leaf to determine the code. And there won't be any encoded
symbol that is a prefix of another one.

For example, with this tree, if we treat the left edge as a "0" and
the right edge as a "1", we get the code above.

```
            +--------+
            |        |
            +--------+
             /      \
            /        \
           /          \
     +----+            +----+
     |    |            |    |
     +----+            +----+
     /    \            /    \
    /      \          /      \
   /        \        /        \
+-----+  +-----+  +-----+  +-----+
|  a  |  |  y  |  |  z  |  |  x  |
+-----+  +-----+  +-----+  +-----+
```


Huffman coding is a way to build these trees and then use them to
encode messages. The basic idea is to compute the frequencies of each
byte that appears in the message and then use those frequencies to
build up the tree.

Lets consider the example input:

```
axyzzyzzzz
```

We first calculate the freqeuencies of each different byte:

-  97(a) : 1
-  120 (x): 1
-  121 (y): 2
-  122 (z): 6

and then we create the leaf nodes of the tree, with no interior nodes
(yet).  So here are the start with four leaf nodes:


```
+-----+  +-----+  +-----+  +-----+
| a:1 |  | x:1 |  | y:2 |  | z:6 |
+-----+  +-----+  +-----+  +-----+
```

Next we find the two nodes that have the lowest frequencies, in this
case `a` and `x`, and we join them together to make an interior node,
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
process, finding the two nodes with the lowest frequencies and joining
them together. In the next step we'll connect our newly created 2 node
with the `y` leaf node:


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
go left and a `1` when we go right. So these are the encoded versions
of the symbols:

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

That is only 16 bits, which is a big savings over the 80 bits for
ASCII. Of course we still have to encode the tree itself which, in
this example, will not end up saving much overall. When the text gets
longer, however, the savings will start to pile up. For example,
repeating the text 10 times will be only 160 bits for the message,
with the same size for the table, but ASCII will require more than 800
bits.

One point to observe here: with block codes like ASCII, we didn't need
to worry about separators between the encoded symbols. Each symbol
uses 8 bits and so we know when we're trying to decode a specific
symbol how many bits to consume. With the Huffman codes, however,
different symbols use different numbers of bits. To avoid problems, we
have a different property, something called prefix codes.

Note we kept adding leaves directly to the top of the tree in this
process, but that doesn't always have to happen. For example, if the
frequences in the original input led to these leaves:

```
+-----+  +-----+  +-----+  +-----+
| a:2 |  | x:2 |  | y:2 |  | z:2 |
+-----+  +-----+  +-----+  +-----+
```

then we would get a tree that's balanced like this:

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

- The file [`bit_io.h`](../../hw/08/src/bit_io.h) contains a number of
  classes to help you read and write bits instead of bytes. It also
  contains some classes to help you test your encoding and decoding
  code.

- Each file will have different frequences and thus a different
  tree. Accordingly, the file has to save the tree (or some
  information that is equivalent to the tree). How should that be
  stored?

- When you are writing out the file, the encoding may not be some
  number of bits that is not an even multiple of 8, which means you
  cannot rely on the information stored in the filesystem to know how
  long the encoded file is. There is more than one good way to handle
  this issue.

# Imperative Binomial Heaps (this time in C++)

To test our understanding of pointers, I've taken a very nice
implementation of binomial heaps that Jesse wrote and turned every
nice kind of pointer into a raw pointer. You can see the results here:
[`Binomial_heap_leaky.h`](src/Binomial_heap_leaky.h). Our job is to
look over this code, remind ourselves how binomial heaps work and then
to figure out and then implement a strategy that avoids the leaks.
