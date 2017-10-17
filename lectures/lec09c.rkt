#lang dssl2

# Functions are not Elephants
# ===========================

# Up until now, any function that we wrote always returns the same
# answer if it gets the same inputs. Indeed, it always runs exactly the
# same computational steps to get that same answer, no matter how many
# times we call it.

# Do all programming languages behave this way? NO.

# What we've done so far is program with immutable values. This is also
# called "pure"; we've programmed in a pure programming language. There
# are many advantages to this kind of programming: it is easier to
# understand in a very technical sense: there are fewer things that can
# happen. If you want to figure out what a function does, you don't have
# to worry about the history of calls to it. Which is a lot less to
# worry about in general!

# But, of course, all "real" programming languages have mutable
# values. That is, you can program in a *state*ful way.

# So we're going to introduce stateful, or imperative programming now.

# How many of you can write a function that accepts a number and returns
# the sum of all of the arguments it has ever received? Lets try to do
# that in DSSL.

# In DSSL, the only thing we are going to mutate are struct fields. So
# in order to have any memory of anything, we have to put that thing
# into a struct. (NB: the contracts are now actual code! Not comments!)

defstruct count (num : int?)
let the_count = count(0)

def sum_args(x : int?) :
    the_count.num = the_count.num+x
    the_count.num

sum_args(1)
sum_args(2)
sum_args(3)
sum_args(4)
sum_args(5)

# --------------------------------------------------
# What does this code do?

def f(a : count?, b : count?) :
    a.num = 2 * b.num

let a_count = count(2)
let another_count = count(3)
let a_third_count = count(4)

f(a_count,another_count)
f(another_count, a_count)
f(a_third_count, another_count)

# --------------------------------------------------
#
# Here's a challenge: without using any arithmetic operators and using
# only the number zero, can you make a call to this function that
# returns 12345?

def f2(a:count?, b:count?) -> int? :
  a.num=12345
  b.num

# Does this work?

f2(count(0),count(0))

# No. How about this?

let yet_another_count = count(0)
f2(yet_another_count,yet_another_count)

# what is going on here? In the past, we said that you could always
# replace global variables with their values and, indeed, that's how
# computation proceeds. But clearly if you take the second program and
# do that, you get the first one. But they produce different results!
# 
# This is, in some sense, the essence of why mutation is complex and why
# we postponed talking about it until now. To understand mutation of
# objects, you really must understand a different notion of
# "same"ness. Two different structs / values are the same if they
# contain the same elements but also the same 
# 
# If I show you a book and then I take it away and I show you another
# book, can you tell if the two books are the same book? You could use
# your phone to take a picture of each page and then check the second
# one against each page. That would tell you if they were the same book
# in some sense. But maybe it is another copy of that book and not the
# same book. How can you tell that?
# 
# Right: you change the book by writing your special secret mark on page
# 42 and then when I give you the book later, you check to see if your
# mark is on page 42.
# 
# That is the same thing with these structs. We have two different
# notions of "the same struct" now. Before we had only one. But now that
# we can mutate particular structs, we have two different notions of
# what it means to be "the same struct".
# 
# It is time to talk about semantics. What is the right way to figure
# out how these programs run?
# 
# The way we do that is to change the way constructors work. We used to
# say that:
# 
#    count(1)
# 
# was a value. But now it isn't anymore. Instead, there is a step of
# computation associated with it. Each time we encounter a call to a
# constructor that has all values as arguments, we lift it out to the
# top of the program in its own special area and give it a special
# kind of name. I'll use a suffix ☺ to indicate these names. The idea
# with this is that these names are different than other names we might
# write in the program so there can never be an overlap with them. Then,
# those names become the values. 

# So: if we have this program:

# let x = count(1)
# x.num=2
# x.num

# ==> it steps first to this; we call this the "allocation" step.
# we write these at the top of the program using ≡ to make them stand out
#
# x☺ ≡ count(1)
# let x = x☺
# x.num=2
# x.count

# ==> now, we treat x☺ as a value, so the next step is to look up x.
#
# x☺ ≡ count(1)
# let x = x☺
# x☺.num=2
# x.count

# ==>  then we can do the assignment operation; it needs a ☺ variable
#      as its first argument; we modify the definition to
#      account for the mutation:
# 
# x☺ ≡ count(2)
# let x = x☺
# x.count

# ==>  now we look up x a second time, and we still get x☺:
# 
# x☺ ≡ count(2)
# let x = x☺
# x☺.count

# ==>  and the selector now operates on ☺ variables too.
# 
# x☺ ≡ count(2)
# let x = x☺
# 2


# Importantly, these smiley variable are bound only to structs, since
# structs are the only things that we mutate.
# 
# Have you heard of "the heap" before? That's what we're doing here. The
# set of definitions at the top of the program that bind the hat
# variables *is* the heap. In a "real" heap, we don't use variables to
# track the values, we use addresses (i.e., numbers). But you don't
# usually program directly with these numbers (adding them or
# multiplying them); instead you think of them as opaque, just as we'll
# do here. We will return to this more when we go to C++
# 
# Getting back to our earlier discussion, can we implement
# those two forms of equality?
# 

let c1=count(0)
let c2=count(1)
let c3=count(0)

def extensionally_equal?(c1 : count?,c2 : count?) -> bool? : 
  c1.num == c2.num

assert_eq extensionally_equal?(c1,c2), False
assert_eq extensionally_equal?(c1,c3), True
assert_eq extensionally_equal?(c2,c3), False
assert_eq extensionally_equal?(c1,c1), True
assert_eq extensionally_equal?(c2,c2), True
assert_eq extensionally_equal?(c3,c3), True

def intentionally_equal?(c1 : count?,c2 : count?) -> bool? :
  let c1_val = c1.num
  let c2_val = c2.num
  let tmp_val = max(c1_val,c2_val)+1
  c1.num = tmp_val
  let ans = c2.num == tmp_val
  c1.num = c1_val
  ans

assert_eq intentionally_equal?(c1,c2), False
assert_eq intentionally_equal?(c1,c3), False
assert_eq intentionally_equal?(c2,c3), False
assert_eq intentionally_equal?(c1,c1), True
assert_eq intentionally_equal?(c2,c2), True
assert_eq intentionally_equal?(c3,c3), True

# It turns out that it is possible to implement intentionally-equal?
# more efficiently (but still constant time) if you have access to
# lower-level representations. Racket provides this via the `eq?`
# function. (Roughly speaking, it compares the "names" of the
# variables in the heap or, if you know something about heaps,
# it compares the addresses.)

# ==== cycles ======================================================
# 
# Using mutation, it now possible to create cyclic structures, something
# that we could not have done before. Check out this program:
# 

# A new data definition: note this is code now!
let linked_list = OrC(link?,False)
defstruct link(num : num?, next : linked_list)

let a = link(0,False)
let b = link(1,False)
let c = link(2,False)
let d = link(3,False)

a.next = b
b.next = c
c.next = d

a


# --------------------------------------------------
# 
# No cycles yet, right? But what happens if we do this:
# 

d.next=a

a

# Will this print out? What will happen?
# 
# What happens if I try to write a function that will sum up the
# elements in the list after making that cyclic link? Lets try it.
# 
# --------------------------------------------------

def sum(l : linked_list) -> num? :
    if l == False :
        0
    else :
        l.num + sum(l.next)
        
assert_eq sum(False),0
assert_eq sum(link(1,link(2,link(3,False)))), 6

# If we try sum(a), tho, it loops! And runs out of memory! Why?
# Well, lets try hand evaluation with this program to see.
# 
# a☺ ≡ link(0,b☺)
# b☺ ≡ link(1,c☺)
# c☺ ≡ link(0,d☺)
# d☺ ≡ link(0,a☺)
#
# sum(a☺)
# =
# 0 + sum(b☺)
# =
# 0 + 1 + sum(c☺)
# =
# 0 + 1 + 2 + sum(d☺)
# = 
# 0 + 1 + 2 + 3 + sum(a☺)
# 
#  ... uh oh.
# 
# Is this fixable? Is there a way we can tell that we've gotten back to
# a place we started from?
# 
# Recall we talked about different notions of equality earlier? If we
# had an accumulator that had all the places we've been in it and we use
# that other notion of equality, then we could solve this problem, right?
# 
# Lets try it. But first, one note about design.
# 
# When we write a mutable function, we should record what it mutates as
# part of the contract/purpose/header step. For example, this is the way
# to write the first mutation program we looked at:
#
#
# EFFECT: modifies `the-count`
# def sum_args(x : int?) :
#     the_count.num = the_count.num+x
#     the_count.num
# 
#
# Okay, now try to implement something that sums up the numbers in a
# potentially cyclic list. Restrict the elements to integers and we can
# do 0, -inf and inf for the sum.


# For a simpler exercise to get used to dssl2, lets say that we
# want to maintain a phonebook database with functions that 
# have these contracts:

# Adds a person to the database
# EFFECT: updates the database to record a new name
def add(s: str?, n: num?) -> VoidC :
  pass

# returns the number of a person in the 
# database, if they are there
def lookup(s : str?, n: num?) -> VoidC :
  pass
  
# With these contracts, we see that there is no actual database
# mentioned anywhere. Why would this be useful to someone actually
# programming?

# Well, one reason is that you are building the back end to some web or
# GUI interface and these are the functions that correspond to clicks in
# the website (and this website isn't implemented using 2htdp/universe
# -- or, perhaps you are implementing 2htdp/universe!)
