#lang dssl2
# Streams
# =======
# 
# DSSL
# 
# what if I wrote this data definition:
# 
#   a Listof-numbers is:
#    (cons Number List-of-numbers)
# 
# what would you say?
# 
# lets make some examples: what happens?
# 
# What if we tried this:
# 
#   (define ones (cons 1 ones))
# 
# that should work, in principle -- why doesn't it? 
# 
# hand evaluation tells us: since we are working on the
# definition for `ones' we cannot yet look up the definition
# for ones.
# 
# What are we hinting at, tho? (infinite list of ones)
# 
# new data definition:

defstruct stream (ele : AnyC, rest : FunC(stream?))

# Look at this for a moment. We've got a function that takes
# no arguments. Strange, no? Hm. Can we make an example?

let ones : stream? = stream(1, λ : ones)

# this time, the definition of ones is already done -- there's
# nothing to be done for either argument of make-stream, so we're
# done.
# 
# Now we're getting closer to that infinite list!
# 
# What's really going on with this function of no arguments?
# Functions really do two things: first they are
# parameterized. But secondly, they make things wait (until
# arguments show up). So far, we haven't focused on that
# aspect of functions, but this example really brings that
# out!
# 
# In this case, we want to wait to lookup the ones until it
# has been defined.
# 
# Using our hand-evaluation rules, we can do this now:
# 
#   one.num = 1
#   one.rest().num = 1
#   one.rest().rest().num = 1
#
# 
# So, what about the template?
# 
#   no condition in the data, so no conditions here! (weird,
#   huh? :)
# 

def stream_template(s : stream?) :
   s.num
   s.rest()

# Lets try to write a function on streams, called add1-stream:
# 

def add1_stream(s : stream?) -> stream? :
    stream(s.ele+1, λ : add1_stream(s.rest()))

let twos = add1_stream(ones)
  
test : assert_eq twos.ele, 2
test : assert_eq twos.rest().ele, 2
test : assert_eq twos.rest().rest().ele, 2

let ListC = OrC(mt?,cons?)
defstruct mt ()
defstruct cons(hd : AnyC, tl : ListC)

# to extract the first `n` elements of `s`
def take(n : nat?, s : stream?) -> ListC :
    if (n == 0) :
        mt()
    else :
        cons(s.ele, take(n-1,s.rest()))
        
test : assert_eq take(0,ones), mt()
test : assert_eq take(1,ones), cons(1,mt())
test : assert_eq take(2,twos), cons(2,cons(2,mt()))


# Can we now define a stream that isn't constant?
# 
# How about this:
# 
#
# let nats = stream(0,add1_stream(nats))
# 
# wrong!
# 
# The second argument to make-stream is _not_ a stream, but it should be
# a function that returns a stream.

let nats = stream(0, λ : add1_stream(nats))

test : assert_eq take(3,nats), cons(0,cons(1,cons(2,mt())))

# Lets generalize add1-stream to map-stream:

def map_stream(f : FunC(AnyC,AnyC), s: stream?) -> stream? :
  stream(f(s.ele),λ : map_stream(f, s.rest()))

# How would we make a stream of the perfect squares?
# 

def sq(x : num?) -> num? :
  x*x

let squares = map_stream(sq,nats)

test : assert_eq take(5, squares), cons(0, cons(1, cons(4, cons(9, cons(16, mt()))))) 

# How about filtering a stream:

def filter_stream(p : FunC(AnyC,bool?), s : stream?) -> stream? :
    if (p(s.ele)) :
        stream(s.ele,λ : filter_stream(p,s.rest()))
    else :
        filter_stream(p,s.rest())

def odd(n : num?) -> bool? : n%2 == 1

test : assert_eq odd(0), False
test : assert_eq odd(1), True
test : assert_eq odd(2), False
test : assert_eq odd(3), True

test : assert_eq take(4,filter_stream(odd,nats)), cons(1, cons(3, cons(5, cons(7, mt()))))


# How about prime numbers?
# 
# Anyone know the Sieve of Eratosthenes?
# 
# Well, we start with a grid of all of the numbers (from 2):
# 

#     2  3  4  5  6  7  8  9 10
# 11 12 13 14 15 16 17 18 19 20
# 21 22 23 24 25 26 27 28 29 30
# 31 32 33 34 35 36 37 38 39 40
# 41 42 43 44 45 46 47 48 49 50
# ... etc

# then we keep the first number, and cross out all 
# multiples of it, leaving us with this grid:

#     2  3     5     7     9   
# 11    13    15    17    19   
# 21    23    25    27    29   
# 31    33    35    37    39   
# 41    43    45    47    49   
# ... etc

# then we take the next number that is still in the grid,
# and we keep it, but cross out all multiples of it:

#     2  3     5     7         
# 11    13          17    19   
#       23    25          29   
# 31          35    37         
# 41    43          47    49   
# ... etc

# and we keep repeating this process forever. The next number
# is 5 and we get this:

#     2  3     5     7         
# 11    13          17    19   
#       23                29   
# 31                37         
# 41    43          47    49   
# ... etc
#
# So, how would we implement this?
# 
# We can filter out numbers that are divisible by a particular
# number, right?
# 

# to remove all numbers divisble by `n` from `s`
def remove_div(n : num?, s : stream?) -> stream? :
  filter_stream(λ x : x%n != 0, s)

let nats2 = map_stream(λ x : x+2,nats)

test : assert_eq take(3, remove_div(2, nats2.rest())), cons(3, cons(5, cons(7, mt())))

# now, we just need a function that does this process for the
# entire stream:

def sieve(s : stream?) -> stream? :
  stream(s.ele, λ : remove_div(s.ele, sieve(s.rest())))

let primes = sieve(nats2)

test : assert_eq take(5,primes), cons(2, cons(3, cons(5, cons(7, cons(11, mt())))))

# There is a performance problem here. In order to see it, lets stick a
# printf into add1_stream that shows what it is adding to:
# 
# def add1_stream(s : stream?) -> stream? :
#     println("adding 1 to ~s", s.ele)
#     stream(s.ele+1, λ : add1_stream(s.rest()))
#
# and then run `take(10,primes)` from the interactions widnow.
# 
# We really don't want to compute that add1-stream so many times. But it
# gets computed EACH TIME we call the function. What can we do about it?
# Get rid of the function? No.... that didn't work at the beginning of
# lecture.
# 
# So: we need something else. A new tool.
