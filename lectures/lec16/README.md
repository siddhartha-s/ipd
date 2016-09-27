# Bloom filter / Pointers

## Bloom filter

Suppose Google Chrome’s website blacklist (for malware sites) contains ten
million URLs, averaging 100 characters in length. How much memory does
it take to represent the the blacklist?

(8 Gbit = 1 GB.)

How long does it take to look up an entry in the blacklist? If we just store 
a sequence of entries then it’s a looong linear scan. What if we want fast 
lookups? We could use a hash table, which lets us look up a URL in constant 
time, and the size of the hash table will be about 1 GB.

1 GB is a lot to download and hold in memory, so in face Google stores the 
hash table on a server, and browsers query it remotely. But that implies a 
network request and response to check the blacklist for each web address that
we load, which makes each load slower. What can we do? Store a *summary of 
the blacklist* on the client.

We don’t need to store any information with each URL, so imagine the 
following: make a hash table where each bucket is just a bit, which will be 
set if occupied and clear if unoccupied. What’s the problem with this approach?

(False positives.)

False positives aren’t necessarily fatal, though. We can look up a URL, and 
if its bit isn’t set then we know it’s not in the blacklist. If the bit is 
set, then we perform the remote query to confirm that the URL is in the 
blacklist. We still have to perform a remote query sometimes, but not on the 
common case, which is that the URL isn’t on the blacklist.

What’s the probability of a false positive?

(It depends on how many bits we use.)

Let *n* be the number of set elements. (In our example, *n* = 10,000,000.) 
Let *m* be the number of bits. Then we expect approximately *n/m* (ignoring 
collisions) of the bits to be set. So when we lookup a URL that isn’t in the 
set, the probability of a false positive is *n/m*. In our example, let’s say 
we want the probability to be 10%. Then we need *n/m = 0.1*, which means
*m = 100,000,000* bits = 12.8 MB. That’s one-eightieth as much space as the 
whole blacklist. It’s perfectly reasonable to download it, and we only need 
to go to the server for 10% of requests, which means 90% of requests are faster.

A Bloom filter does somewhat better than this by using multiple hash 
functions. Let the hash functions be *h1*, *h2*, etc. Then to add an element 
*s* to the set, we set the bits indicated by *h1(s)*, *h2(s)*, and so on. 
Then to look up an element, we check whether all the bits indicated by the 
hash functions are set. If all the bits are set, then the element is 
possibly (probably) in the set; if any of the bits is clear then the element 
definitely isn’t in the set.

Note that we can’t remove elements because multiple elements may share some 
same bits.

## The code

The interface for a Bloom filter is in `src/Bloom_filter.h`. To construct a 
Bloom filter, we give the constructor the number of bits and the number of 
hash functions. There are operations to insert a string into the filter and 
to check whether a string is in the filter.

We represent the Bloom filter using a `std::vector<bool>` for the bits and a 
`std::vector<Sbox_hash>` for the hash functions. Then the operations (defined
 in `src/Bloom_filter.cpp`) are straightforward:
 
  - The constructor adds `nfunctions` hash functions to the hash function 
  vector.
  
  - The `insert` function loops over the hash functions and sets the bit 
  corresponding to each.
  
  - The `check` function loops over the hash functions, and if any hashed bit
  isn’t set then it returns `false`. If all the checked bits are set then it
  returns `true`.

## The math

How likely are false positives when using multiple hash functions? Consider:

  - The probability of one hash function setting one particular bit is 1/m. Or, 
  the probability of a bit not being set by the hash function is 1 - 1/m.
  
  - If there are k hash functions, then the probability of a bit being not 
  set is (1 - 1/m)^k.
  
  - If we insert n elements, then the probability of a bit not being set is
  (1 - 1/m)^(kn), or the probability of a bit being set is
  1 - (1 - 1/m)^(kn).
  
  - Now suppose we lookup an element that's not in the set. That means we 
  check k bits, and we return true only if all k bits are set. So the 
  probability that all k bits are set is
  [1 - (1 - 1/m)^(kn)]^k.
  
Let's try a particular instance of this with our example. So n = 10,000,000. 
Let's use m = 100,000,000 as before, and let the number of hash functions k = 7.
Then plugging in the numbers, we get 0.008, or about 1%.
  
The optimal number of hash functions k = (m/n) ln 2. In that case, then using
4.8 bits per entry gets us a false positive rate of 10%, 9.6 bits per entry 
gets us a false positive rate of 1%, and so on for ever additional 4.8 bits 
per entry eliminates 90% of the remaining false positives.

## Pointers

