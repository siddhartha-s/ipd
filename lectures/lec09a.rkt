#lang dssl2

# Intro to DSSL2
# --------------
#
# Today we switch to a new language, dssl2. To install it, in DrRacket
# choose the File | Install Package... menu item. Type “dssl2” into the
# dialog that appears and click “Install”. Wait. When the “close” button
# appears, click it. 
#
# Next, choose the “Language | Choose Language...” menu item. Select
# “The Racket Language” and then hit “OK” and type “#lang dssl2” as the
# first line in the file. Congrats! You are now in the data-structures
# student language, version 2.
#
# DSSL2 is a language whose syntax follows python, including being
# whitespace sensitive. Also unlike BSL and ISL, it has signatures
# and data definitions as actual constructs in the language.
#
# Read the documentation for the language. To do so, type F1 and then
# type “dssl2” into the resulting search (or go to https://docs.racket-lang.org
# in your browser and type “dssl2” there). There are also some examples
# of the syntax below.

# To get us started, lets do binary search trees again, but this time
# to explore DSSL.

# Old data definition:
#
# A binary-search-tree is either
#   #false, or
#   (make-node number? binary-search-tree binary-search-tree)
# (define-struct node (num left right)
# INVARIANT: num < smallest number in `right`
#            and larger than largest in `left`
#
# New data definition (now with 50% fewer comments!)
let binary_search_tree = OrC(False,node?)
defstruct node (num   : num?, 
                left  : binary_search_tree, 
                right : binary_search_tree)
# INVARIANT: num < smallest number in `right`
#            and larger than largest in `left`

# example trees:

let t1 : binary_search_tree = node(1, node(0, False, False), node(2, False, False))
let t2 : binary_search_tree = node(0, False, node(1, False, node(2, False, False)))
let t3 : binary_search_tree = node(2, node(1, node(0, False, False), False), False)

# Lets compare the lookup function; note that the signature is now in the code itself.

# to determine if `n` is in `t`
def lookup(t : binary_search_tree, n : num?) -> bool? :
    if t == False :
        False
    else :
        if n < t.num :
            lookup(t.left,n)
        elif n == t.num : 
            True
        else :
            lookup(t.right,n)

# examples as tests

test 'lookup tests' :
    assert_eq lookup(t1,-1), False
    assert_eq lookup(t1,0),  True
    assert_eq lookup(t1,1),  True
    assert_eq lookup(t1,2),  True
    assert_eq lookup(t1,3),  False
    assert_eq lookup(t2,-1), False
    assert_eq lookup(t2,0),  True
    assert_eq lookup(t2,1),  True
    assert_eq lookup(t2,2),  True
    assert_eq lookup(t2,3),  False
    assert_eq lookup(t3,-1), False
    assert_eq lookup(t3,0),  True
    assert_eq lookup(t3,1),  True
    assert_eq lookup(t3,2),  True
    assert_eq lookup(t3,3),  False
    
# if we try an example in the interactions window that violates the contract:

# > lookup('a',False)
# lookup: contract violation
# expected: "OrC(False, node?)"
# given: 'a'
