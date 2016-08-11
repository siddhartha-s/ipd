;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname aa) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|

Persistent AA trees

An AA tree is a form of binary search tree that maintains the overall
shape of the tree (and thus good algorithmic properties) during insert
operations.

Here's the datastructure for it:

|#

;; an X AA-node is either:
;;   - "leaf"
;;   - (make-node X natural-number AA-node AA-node)
(define-struct node (value level left right))

#|

As compared with the normal binary search tree, each node has an
additional natural number, the level.  There are a number of rules
that govern the relationship between the values of the level field of
a parent and its children. We collectively call these rules the AA
invariant. They are:

   - the level of a left-child is one more than the level of the
     parent

   - the level of a right child is either the same as the parent or
     one more. If it is the same, then the right child of the right
     child must be one more (so no two consecutive right children can
     have the same level)

where the level of a leaf node counts as 0.

And, of course, the binary search tree invariant also holds:

  - the values of all nodes in a left child are less than the value in
    the node

  - the values of all nodes in a right child are less than the value
    in that node

|#


;; Examples.

;; Define an example AA tree (holding numbers) that
;; has only the number 0 in it.
(define zero "...")

;; Define the only AA tree that has the numbers 1, 2, and 3 in it.
(define one-two-three "...")

;; There are two AA trees that have the numbers 1, 2, 3, and 4 in them.
;; Define them both. (The constraint that all "leaf"s have level 0
;; means that many of the trees that you might think are AA trees
;; really are not.)
(define one-two-three-four-a "...")
(define one-two-three-four-b "...")

;; Define a tree that has at least 6 numbers in it.
(define six-nodes "...")

#|

Our next goal is to design an insertion function.

Start by designing a function that inserts a number into a given tree
without regard for the 'level' invariants, paying attention only to
the binary search tree invariant.  It should insert the new value in
the place of some leaf node (with level 1) and leave all of the other
levels alone

|#

;; insert-wrong : (X AA-tree) X (X X -> Boolean) (X X -> Boolean) -> (X AA-tree)
;; inserts 'value' into 'tree' using 'less-than' and 'equal-to' without
;; regard to the AA invariant.
(define (insert node value less-than equal-to) "...")


#|

Next, find two example AA-trees and numbers such that, when passed to
insert-wrong, each returns a tree that is not an AA-tree. One of the
examples should violate the constraint that the left child is one
level lower than the parent and the other should violate the contraint
that the right-right grandchild is (at least) one level below the parent.

Add these two examples as test cases.

|#



#|

The invariant-preserving version of AA tree insert looks very much like
incorrect one, except that, each time it recursively processes a
"node", it performs two additional operations, a 'skew' and a 'split' on
the result before returning it.

In order to define those operations we first need to define two helper
functions, rotate-left and rotate-right.

The rotate-left function adjusts the nodes in a BST in a way that
preserves the ordering but changes the shape of the tree.
Specifically, if you have a tree with three nodes (A B C D) at
the top and then four trees (t1 t2 t3 t4) below them in this shape:

     B
    / \
   /   \
  A     C
 / \   / \
t1 t2 t3 t4

then you can rotate it to the left to produce this tree:

      C
     / \
    B   t4
   / \
  A   t3
 / \ 
t1 t2

If the first tree supported the binary search tree invariant, then so
does this tree. As you can see, in the original tree, we had A < B < C
and that also holds in the new tree. Verify for yourself that the
elements of the subtrees (t1 t2 t3 t4) are okay. For example, you know
that the elements of t2 must all be bigger than A and less than B from
their position in the first tree. Is that the case in the second?
Does that work for the other subtrees?

The rotate-right function does the same thing, but in the other
direction.  Show what a right rotation looks like on this (same) tree:


     B
    / \
   /   \
  A     C
 / \   / \
t1 t2 t3 t4

Design the rotate-right and rotate-left functions.

|#

;; rotate-right : X AA-node -> X AA-node
;; assumes at least two 'node's


;; rotate-left : X AA-node -> X AA-node
;; assumes at least two 'node's


#|

After finishing the rotation functions, we're ready to design split
and skew. Skew is slightly simpler so we start with it: it accepts a
tree and, if the tree is a node and if the left subtree has the same
level as the original tree (which would violate the AA invariant),
then it performs a rotate right.

Split accepts a tree and, if the tree is a node and if the right
subtree is also a node, and the right subtree of the right subtree has
the same level as the tree (which would violate the AA invariant),
then it performs a rotate-left and it increments the level of the
result of the rotation.

(Don't forget: the leaf node counts as having a level 0.)

|#


#|

Finally, we're ready to design insert. For the first step, look at
the pdf slides here:

  http://faculty.ycp.edu/~dbabcock/PastCourses/cs350/lectures/AATrees.pdf

They show a series of trees that you get (and the skews and splits) by
inserting the numbers (in order): 6, 2 8, 16, 10, and 1. Write each of
those down as test cases and then finish the design of insert.

|#



;; insert : (X AA-tree) X (X X -> Boolean) (X X -> Boolean) -> (X AA-tree)
;; inserts 'value' into 'tree' using 'less-than' and 'equal-to'
(define (insert node value less-than equal-to) "...")
