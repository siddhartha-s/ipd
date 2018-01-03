# Hashing and Hash Tables

A hash table provides a (non-persistent) version of the
dictionary API:

```cpp
  // hash<K,V> new()
  // creates a new hash table that maps K to V

  // void add(hash<K,V> hash, K key, V value)
  // mutates `hash` so that it maps `key` to `value`

  // bool member(hash<K,V> hash, K key)
  // determines if `key` is in `hash`

  // K lookup(hash<K,V> hash, K key)
  // returns the value mapped by the key, raising
  // an error if `key` is not mapped by the table
```

The basic idea of the hash table implementation hinges on the ability
to map the keys (`K`s) to natural numbers. We then use those natural
numbers as indices into a vector and store key/value pairs in the
vector.

The mapping to natural numbers is called "hashing" and it will not
always map every different object to a different natural number. That
is, sometimes it will map two keys to the same number (this is called
a "collision") and so each entry in the vector is going to itself
contain a vector, which we will just scan to find the entry we want.

Hash tables generally behave differently at different sizes of the
vector, so we'll make two constructors; one that uses a default size
and one where we can specify the size of the vector

```cpp
  // hash<K,V> new(size_t size)
  // creates a new hash table that maps K to V
  // with an internal vector of size `size`

  // hash<K,V> new()
  // equivalent to new(10000)
```

For the rest of this lecture, we'll assume that the keys are strings,
just to ease the discussion, but everything we can do will work for
other kinds of values, too, with some care. Generally, the way this
works is to map the structure to a sequence of bytes and then to use
the techniques discussed today.

So, let's say that our hashing function maps every key to the value of
its first letter in ASCII, and let's say that these calls happen:

```cpp
   Hash<std::string,int> ht(4);
   ht.add("aorta",0);
   ht.add("bob",1);
   ht.add("cost",2);
   ht.add("dog",3);
 ```

Then we would get a vector like this (using `[,]` to notate the vectors
and `<,>` to notate the key/value pairs):

```c++
  [[<"dog",3>],
   [<"aorta",0>],
   [<"bob",1>],
   [<"cost",2>]]
```

But, if our hashing function instead used the second letter of the
key, in ASCII, we would get this:

```c++
  [[],
   [],
   [],
   [<"dog",3>,<"cost",2>,<"bob",3">,<"aorta",4>]]
```

Which one of these is better for `lookup`? (first!)

Let's talk about the running time of the operations. Going in order:

- `new()` -- linear (in the size of table)

- `lookup()` -- anywhere from constant to linear (in the number of
   elements in the table), depending how good the hashing function is
   and how big the table is

- `member()` -- the same as `lookup()`

- `add()` -- also the same as `lookup()`, since we need to scan the
  entire sequence that is in the bucket to see if we are replacing an
  existing mapping or not

Basically, the linear case is when all of the elements have
the same hash code ("hashed to the same bucket") and the constant
case is when they are all going into different buckets.

Lets look at some code that implements the hashing functions,
[`Vec_hash.h`](src/Vec_hash.h) and some code that we'll use as a
client of the hash table, [`hamlet.cpp`](src/hamlet.cpp).

## How can we make a good hashing function?

Okay, the `hash` function in that code is poor. Lets make a better
one. But first, we should try to figure out how well the hashing
function is working so we know if we're improving things or not.

Lets see if we can write some code to check to see which of the two
situations we are in, above. How can we do that?

 ... <write some code>

And lets write some new hashing functions that maybe do a bit
better. I've put a very bad hashing function in Various_hashes.h to
show how we can swap out the hashing function for another one. Lets
take a look. Lets make up a few more hashing functions to see if we
can do better.

 ... <write some code>

Hashing functions generally consist of three conceptual pieces: some
internal state (that is generally the size of the result of the hash
function), a function that mixes up the internal state, and a function
that pulls in a chunk of the data being processed and combines it with
the internal state.

 ... <write some code>

Stepping back, there are three properties of hash functions that we
care about for hash functions:

- **uniform distribution**: two different inputs should map to two
  different outputs. Of course, this isn't possible if the number of
  inputs is larger than the number of outputs, but we want to minimize
  collisions, so we want to avoid "bunching up". In other words, we
  want to make sure that all of the outputs are equally likely to be
  used.

- **one way**: hashing functions are used for hash tables, but also for
  other purposes that are security sensitive. In those cases, it is
  important that we cannot easily go from a hashed value to an input
  to the hash function. For example, passwords are not stored; instead
  the hash of a password is stored. If someone gets that hash, we
  don't want them to find out what the password was.

  This property is also important for security in hash tables; we'll
  return to it later in the lecture.

- **avalance**: we want inputs that are only a little bit different to
  hash to completely different outputs. The issue here is that the
  keys in any given hash table are usually not very random (think of a
  hash table that stores URLs or stores English text, etc) and so we
  want little changes in the input to correspond to large changes in
  the output.

## Avalanche

Avalanche is an interesting one to measure. Right now, we have hashing
functions that produces 64 bits, but lets simplify that a little bit
and look at some hashing functions that operate only on 4 bits and
lets look at the "add1" function. This function is NOT one-way. But
okay, on a 4 bit hashing function, a table lookup will go the other
way so there aren't really any one way functions, so lets ignore that
one for now. It is bijective, so the first point is satisfied. How
about avalanche?

The way we measure avalance is to say: if I perturb one bit of the
input, how many bits flip in the output? As we want that to, for the
most part, be half of the available bits. 

Here is the add1-mod-16 function, written out on the binary
representation of four-bit numbers:

```c++
0000  -->  0001
0001  -->  0010
0010  -->  0011
0011  -->  0100
0100  -->  0101
0101  -->  0110
0110  -->  0111
0111  -->  1000
1000  -->  1001
1001  -->  1010
1010  -->  1011
1011  -->  1100
1100  -->  1101
1101  -->  1110
1110  -->  1111
1111  -->  0000
```

and we can build a table that tells us, for each bit we might flip in
the input, how many times does an output bit flip?

```c++
     output flip:  0   1   2   3
input flip:
 0                 8   8   4   2
 1                 0   8   4   2
 2                 0   0   8   2
 3                 0   0   0   8
```

What we want here is to get 4s everywhere. This is far from that, so
we would say that this function is not very good on the avalanche
criterion.

Here's another function (that I cannot describe easily, except by
writing it down):

```c++
0000 -> 1111
0001 -> 1010
0010 -> 0000
0011 -> 0010
0100 -> 1001
0101 -> 1011
0110 -> 0100
0111 -> 1000
1000 -> 0111
1001 -> 1110
1010 -> 0011
1011 -> 1101
1100 -> 1100
1101 -> 0101
1110 -> 0110
1111 -> 0001
```

This one achieves perfect avalanche:

```c++
     output flip:  0   1   2   3
input flip:
 0                 4   4   4   4
 1                 4   4   4   4
 2                 4   4   4   4
 3                 4   4   4   4
```

When we want to measure avalanche for functions on larger numbers of
bits, it is not really feasible to build these tables. For N bits, we
have an N^2 matrix and each entry in the matrix requires O(2^(N))
calls to the hash function, plus the work to check and see which bits
flipped. There may be a more efficient way to compute the matrix, but
fundamentally it requires us to compare each result of the hash
function with 2^(N-1) other results, so that's going to take a long
time.

Instead, what we can do is randomly pick a bit string and then compute
which bits flip in the output of the hash when we flip each bit of the
input. This will give us an approximation to the content of the
avalanche matrix and we can keep picking random bit strings and trying
until things seem to settle down.

The code in [`avalanche.rkt`](src/avalanche.rkt) repeatedly calls the
code in [`avalanche.cpp`](src/avalanche.cpp) to compute this for the
hash function listed in [`avalanche.cpp`](src/avalanche.cpp). Lets
try a few of these out to see how well we are doing wrt to avalanche
behavior.

One way to get good avalanche behavior is to make a random permutation
of bytes and then use that in the hash function. That is, we can "mix
up" the bits that come into our function by passing it through a
random byte-to-byte bijection. Because it is a bijection, we will not
compromise the other properties of the hash function and because it is
random, we will get good "mixing" of the bits. Here's some code that
does that.

```cpp

#include <random>

template<typename T>
class Sbox_hash : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    Sbox_hash(size_t = Vec_hash<T>::default_size);

private:
    // add a private field that stores
    // the bijection as a lookup table
    std::array<size_t, 256> sbox_;
};

template<typename T>
Sbox_hash<T>::Sbox_hash(size_t size) : Vec_hash<T>(size)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;
    for (auto& n : sbox_) n = dist(rng);
}


template<typename T>
size_t Sbox_hash<T>::hash(const std::string& s) const
{
    size_t hash = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        hash ^= sbox_[(unsigned char)s[i]];
        hash *= 3;
    }

    return hash;
}
```

## Denial of service implications

Webservices will store parts of the input they get from the web in
they are given in hash tables. Even if you, as the web services
provider, doesn't ask for it to be saved. So, an attacker can supply
an input to the web server that causes it to fill up just a single
bucket in the hash and now every operation with the hash requires the
linear scan and this can, with one request to the webserver, keep the
cpu pegged for 40 minutes(!).

https://events.ccc.de/congress/2011/Fahrplan/attachments/2007_28C3_Effective_DoS_on_web_application_platforms.pdf

https://www.youtube.com/watch?v=R2Cq3CLI6H8

##  Other hash functions in the literature:

- [CityHash](https://github.com/google/cityhash)
- [SipHash](https://131002.net/siphash/siphash.pdf)
- [SpookyHash](http://www.burtleburtle.net/bob/hash/spooky.html)

## Open addressing

Instead of using chaining, we can keep only a std::vector<Entry>. The
basic idea is to find the position in the arry where the value would
hash to. If it is empty, we fill it. If it is full, we just start
moving through the array until we find an empty spot and then use that
spot.

Lets code that up.


## Inspiration for this lecture:

[http://papa.bretmulvey.com/post/124027987928/hash-functions](http://papa.bretmulvey.com/post/124027987928/hash-functions)

[http://papa.bretmulvey.com/post/124028832958/hash-functions-continued](http://papa.bretmulvey.com/post/124028832958/hash-functions-continued)

