#lang dssl2

# To use the `dssl` language, use the "Language | Choose Language"
# dialog to select "The Racket Language".
#
# You also need to install the language. Go to the "File | Install Package..."
# menu item in DrRacket. Type "dssl" in the box and click "Install".

# We import cons to get:
#
#   A ListOf[X] is one of:
#    - nil()
#    - cons(X, ListOf[X])
#   defstruct nil()
#   defstruct cons(car, cdr)
#
#   cons_of_vec : VectorOf[X] -> ListOf[X]
#   (Like ISL's `list` function, but takes a vector)
import cons

# A Stream is:
#   stream(Number, FunC(Stream))
defstruct stream(num, rest)

# take : Nat Stream -> ListOf[Number]
# to find the nth element of the stream.
#
# Strategy: struct decomp. (nat)
def take(n, stream):
    if zero?(n): nil()
    else: cons(stream.num, take(n - 1, stream.rest()))
    
let ones = stream(1, λ: ones)

#
# EXERCISE 1
#

# Define the stream of all perfect squares.
# The first few numbers are:
#   0 1 4 9 16 25 36 49 64 81.

let squares = ones  ## wrong!

test 'first 10 squares':
    assert_eq take(10, squares), \
        cons_from_vec([0, 1, 4, 9, 16, 25, 36, 49, 64, 81])

#
# EXERCISE 2
#

# Define a stream that has all of the even integers.
# This is a good way to organize that stream:
#
#    0 2 -2 4 -4 6 -6 8 -8 10 -10 12 -12
#
# (but it isn't the only way). 

let evens = ones  ## wrong!

# You may have to change this test if you produce the
# evens in a different order:
test 'first 10 evens':
    assert_eq take(10, evens), \
        cons_from_vec([0, 2, -2, 4, -4, 6, -6, 8, -8, 10])

#
# EXERCISE 3
#

# The Fibonacci numbers start with 0, then 1, and each
# number aftewards is the sum of the previous two
# numbers. Here are the first few numbers:
#
#   0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377
#
# Using the stream data definition, define a stream
# that consists of all of the fibonnaci numbers.

let fibs = ones  ## wrong!

test 'first 15 fibs':
    assert_eq take(15, fibs), \
        cons_from_vec([0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,
                       89, 144, 233, 377])
