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

1 GB is a lot to download and hold in memory, so in fact Google stores the 
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
gets us a false positive rate of 1%, and so on for every additional 4.8 bits 
per entry eliminates 90% of the remaining false positives.

## A linked list

And now for something completely different...

### Story 1

C++ classes and structs have a *fixed size*. For example, here is our 
two-fish aquarium:

```
struct aquarium
{
    fish fst;
    fish snd;
};
```

How do you think we would make an aquarium that holds a variable number of 
fish? (Assume we can’t use `std::vector` because that uses the mechanism 
we’re exploring right now.) We might use the BSL linked list solution, like so:

```
struct aquarium
{
    fish first;
    aquarium rest;
}
```

The problem with this is that unlike BSL boxes, C++ boxes don’t grow. How big
is aquarium in bytes? Well, it's the size of fish plus the size of aquarium:

```
    sizeof(aquarium) == sizeof(fish) + sizeof(aquarium)
```
    
This has no solutions if `sizeof(fish)` is non-zero. Instead, we add an 
indirection using a *pointer*, which is a fixed-size object that can refer to
another (often larger) object:

```
struct aquarium
{
    fish first;
    std::shared_ptr<aquarium> rest;
}
```

A `std::shared_ptr` has a size that does not depend on the size of 
`aquarium`, so now

```
    sizeof(aquarium) == sizeof(fish) + sizeof(shared_ptr)
```

Now `sizeof(aquarium)` is well defined.

We create an aquarium with `std::make_shared<aquarium>(ARGS)` where `ARGS` 
are the arguments to pass to the `aquarium` constructor. This allocates the 
new aquarium and gives us a pointer referring to it.

There’s another problem that using `shared_ptr` solves: What do we put at the
end of the list? Well, `shared_ptr` includes a special value `nullptr` that 
refers to nothing. So that’s what we put in the rest field for the last node 
of the list.

```
std::shared_ptr<aquarium> a = make_shared<aquarium>(one_fish, nullptr);
std::shared_ptr<aquarium> b = make_shared<aquarium>(two_fish, a);
std::shared_ptr<aquarium> c = make_shared<aquarium>(red_fish, b);
std::shared_ptr<aquarium> d = make_shared<aquarium>(blue_fish, c);
```

### Story 2

C++ variable declarations have *block scope*:

```
void f(int x)
{
    std::string y;
    
    for (int i = 0; i < len_of_something; ++i) {
        std::vector<double> d;
        stuff();
    }
}
```

Variable `x` lasts longer then `y` which lasts longer than `i` which lasts 
longer than `d`, so we can allocate them on a stack:

```
(old end)
[  x  ]
[  y  ]
[  i  ]
[  d  ]
```

But what if we want to allocate a variable that lasts longer than its block? 
There’s another place we can allocate, called the *heap*, and we do it with
`make_shared`. Then we can return the pointer, and so long as the pointer 
exists, it will keep the variable on the heap alive as well.

### A linked list struct

See `src/Cons_list.h` for a minimal, BSL-like linked-list class. The main 
definition of the struct is something like:

```
struct Int_cons
{
    const int first;
    const std::shared_ptr<Int_cons> rest;

    Int_cons(int, const std::shared_ptr<Int_cons>&);
};
```

(It includes a constructor, because `make_shared` will want a constructor to 
call.) In order to avoid referring to the long name 
`std::shared_ptr<Int_cons>` repeatedly, we use a type alias:

```
using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    const int first;
    const Int_list rest;

    Int_cons(int, const Int_list&);
};
```

Note that this creates a chicken-and-egg declaration problem, since 
`Int_list` refers to `Int_cons` and `Int_cons` refers to `Int_list`. To break
this cycle, we *forward declare* the existence of `Int_cons` before defining
`Int_list`, which tells C++ that `Int_cons` will be defined in the future, 
but doesn’t define it just yet:

```
struct Int_cons;

using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    const int first;
    const Int_list rest;

    Int_cons(int, const Int_list&);
};
```

In order to work with lists, we declare three helper functions following BSL:

```
Int_list cons(int, const Int_list&);
int first(const Int_list&);
const Int_list& rest(const Int_list&);
```

See the definitions of these functions in `src/Cons_list.cpp`.

Notice that when `p` is a pointer to a struct with member `x`, we refer to `x` 
as `p->x`.

## Constant-time FIFO

We can use a linked list to implement a FIFO queue with constant time 
operations. See `src/SP_queue.h` for an example. The `Queue` class defines a 
private member class `node_` to hold the data. Then the private members of 
`Queue` are:

```
    struct node_;
    using link_t = std::shared_ptr<node_>;

    link_t head_;
    link_t tail_;
```

That is, a `Queue` will be stored as two `shared_ptr<node_>`s, one a 
reference to the first node of the list and one a reference to the last. This
lets us remove elements at the head and add new nodes at the tail.

Note, next, how `node_` is defined outside the `Queue` class, but qualified 
by it:

```
template <typename T>
struct Queue<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    link_t next;
};
```

Read this as “for all types `T`, a `Queue<T>::node_` is…”

The `enqueue` function uses `make_shared` to allocate a new node and links it
into the queue. The `dequeue` function returns the element of the first node 
and updates the head pointer to point to the next node. Both have some 
additional code for dealing with the empty case.

### Other kinds of pointers

When using `std::shared_ptr`, we can make as many copies of the pointer as we
like, and C++ ensures that the pointed-to object is freed when all references
go away. There are other kinds of pointers we can use that have the same 
purpose in indirecting a reference to a variable, but manage the memory of 
the variable differently.

A raw pointer type, written `T*` for a pointer to type `T`, does not manage 
the memory that it points to at all. In `src/RP_queue.h` is a version of the 
`Queue` class that uses raw instead of shared pointers. In order to allocate 
a node, it uses `new node_(ARGS)`. It also has to explicitly `delete` nodes 
when it is done with them.

Finally, there is a unique pointer type `std::unique_ptr<T>`, which does not 
allow multiple references, and frees the memory as soon as it goes away. We 
can build our list using unique pointers, but we cannot have a second unique 
pointer for the tail of the list. That is, the last node of the list has two 
pointers to it: one from the previous node, and one from the `tail_` member 
of `Queue`, and they cannot both be unique. Instead, the main pointers of the
list are unique, and the tail pointer is a raw pointer to the same tail node.
Given a unique pointer `p`, we get the raw pointer to the same memory as `&*p`.
