# Hashing and Hash Tables

A hash table provides a (non-persistent) version of the
dictionary API:

```C
  // hash<K,V> new()
  // creates a new hash table that maps K to V

  // void add(hash<K,V> hash, K key, V value)
  // mutates `hash` so that it maps `key` to `value`

  // bool (member hash<K,V> hash, K key)
  // determines if `key` is in `hash`

  // T lookup(hash<K,V> hash, K key)
  // returns the value mapped by the key, raising
  // an error if `key` is not mapped by the table

```

The basic idea of the hash table implementation hinges on the ability
to map the keys (K)s to natural numbers. We then use those natural
numbers as indicies into a vector and store key/value pairs in the
vector.

The mapping to natural numbers is called "hashing" and it will not
always map every different object to a different natural
number. Sometimes, it will map two keys to the same number (this is
called a "collision") and so each entry in the vector is going to
itself contain a vector, which we will just scan.

Hash tables generally behave differently at different sizes of the
vector, so we'll make two constructors; one that uses a default size
and one where we can specify the size of the vector

```C
  // hash<K,V> new(int size)
  // creates a new hash table that maps K to V
  // with an internal vector of size `size`

  // hash<K,V> new()
  // equivalent to new(500)
```

For the rest of this lecture, we'll assume that the keys are strings,
just to ease the discussion, but everything we can do will work for
other kinds of values, too, with some care.

So, lets say that our hashing function maps every key to the value of
its first letter in ASCII, and lets say that these calls happen:

```
   Hash<std::string,int> ht(4);
   ht.add("aorta",0);
   ht.add("bob",1);
   ht.add("cost",2);
   ht.add("dog",3);
 ```

Then we would get a vector like this (using [,] to notate the vectors
and <,> to notate the key/value pairs):

```
  [[<"dog",3>],
   [<"aorta",0>],
   [<"bob",1>],
   [<"cost",2>]]
```

But, if our hashing function instead used the second letter of the
key, in ASCII, we would get this:

```
  [[],
   [],
   [],
   [<"dog",3>,<"cost",2>,<"bob",3">,<"aorta",4>]]
```

Which one of these is better for `lookup`? (first!)

Lets talk about the running time of the operations. Going in order:

-  new-- constant (size of table)

-  add-- constant (assuming hash function is constant time)

-  member -- also constant (also assuming...)

-  lookup -- anywhere from constant to linear, depending how good the hashing function is.

Lets explore some hashing functions. 



What makes a hashing function good?

distribution

crypto

avalance()

spooky hash's short() function

--- security implications via denial of service attacks.

---  