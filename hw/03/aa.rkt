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

;; An [AA-tree X] is:
;;  (make-tree [AA-node X] Natural [X X -> Boolean])
(define-struct tree (root size less-than))

;; An [AA-node X] is one of:
;;   - "leaf"
;;   - (make-node X Natural [AA-node X] [AA-node X])
(define-struct node (value level left right))

#|
The less-than operation must be a strict comparsion operation. That is,
it must be the case that, for any two possible x1 and x2 that are both Xs,
at most one of these two expressions can return #true:

  (less-than x1 x2)
  (less-than x2 x1)

This means that you can compare any two elements in the tree with a
`cond` expression like this one:
|#

#;
(cond
 [(less-than x1 x2) ...] ;; x1 < x2 
 [(less-than x2 x1) ...] ;; x1 > x2
 [else ...])             ;; x1 = x2


#|
As compared with the normal binary search tree, each node has an
additional natural number, the level.  There are a number of rules
that govern the relationship between the values of the level field of
a parent and its children. We collectively call these rules the AA
invariant. They are:

   - the level of a left-child is one less than the level of the
     parent

   - the level of a right child is either the same as the parent or
     one less. If it is the same, then the right child of the right
     child must be one less (so no two consecutive right children can
     have the same level)

where the level of a leaf node counts as 0.

And, of course, the binary search tree invariant also holds:

  - the values of all nodes in a left child are less than the value in
    the node

  - the values of all nodes in a right child are less than the value
    in that node

|#


;; Examples.

;; Define the example [AA-tree number] that has only the number 0
;; in it and uses < and = as the less than and comparison operation.
(define zero "...")

;; There is only one [AA-tree number] that has the numbers 1, 2, and 3 in
;; it, using < and = as the less than and comparison operation. Define it.
(define one-two-three "...")

;; Similarly, there is only one that has the strings "one" "two" and "three"
;; in it, using string<? as the comparison predicate. Define it.
(define one-two-three-string "...")

;; There are two [AA-tree number]s that have the numbers 1, 2, 3, and
;; 4 in them (when using < and =). Define them both. The constraint
;; that all "leaf"s have level 0 means that many of the trees that
;; you might think are AA trees really are not.
(define one-two-three-four-a "...")
(define one-two-three-four-b "...")

;; Define an [AA-tree number] that has at least 6 numbers in it.
;; Use > and = as the comparsion and equality.
(define six-nodes "...")

#|

The first function we are going to design is a lookup function.
It should look in the tree only in places that might have the
number. In other words, be sure that the function you write takes
advantage of the binary search tree invariant.

|#

;; lookup : [AA-tree X] X -> Boolean
;; to determine if `x` occurs in `tree`
(define (lookup tree x) "...")

#|

Our next goal is to design an insertion function.

Start by designing a function that inserts a number into a given tree
without regard for the 'level' invariants, paying attention only to
the binary search tree invariant.  It should insert the new value in
the place of some leaf node and leave all of the other levels alone.
The new node should have level 1.

|#
;; insert-wrong : [AA-node X] X [X X -> Boolean] -> [AA-node X]
;; inserts 'value' into 'tree' using 'less-than' without
;; regard to the AA invariant.
(define (insert-wrong node value less-than) "...")

#|

Next, find two example AA-nodes and numbers such that, when passed to
insert-wrong, each returns a tree that is not an AA-tree. One of the
examples should violate the constraint that the left child is one
level lower than its parent and the other should violate the contraint
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

;; rotate-right : [AA-node X] -> [AA-node X]
;; ASSUMPTION: `node` is not "leaf", and `(node-left node)` is not "leaf"


;; rotate-left : [AA-node X] -> [AA-node X]
;; ASSUMPTION: `node` is not "leaf", and `(node-right node)` is not "leaf"


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
inserting the numbers (in order): 6, 2, 8, 16, 10, and 1. Write each of
those down as test cases (i.e. write them down as [AA-node X]s)
and then finish the design of insert.

Note that when you write test cases, you are not allowed to pass functions
(or structures that have functions inside them) to check-expect. So,
for example, this:

  (check-expect (make-tree "leaf" 0 < =) (make-tree "leaf" 0 < =))

will raise an error. To avoid this problem, write test cases that check
only the root field and size field of your insertion function (by calling
selectors on the result of insert).

|#

(define inserted-nothing
  "...")
(define inserted-6
  "...")
(define inserted-6-2
  "...")
(define inserted-6-2-8
  "...")
(define inserted-6-2-8-16
  "...")
(define inserted-6-2-8-16-10
  "...")
(define inserted-6-2-8-16-10-1
  "...")

;; insert : [AA-tree X] X -> [AA-tree X]
;; inserts `x` into `tree`

