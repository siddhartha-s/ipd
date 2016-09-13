# Class abstraction

## Part 1: Classes and privacy

### Bank account example

Suppose we want to represent a bank account, with an account number, and 
owner, and a balance. We might define a struct like this:

```
struct Bank_account
{
    unsigned long id;
    std::string owner;
    unsigned long balance;
}
```

Now you can create an account like so:

```
Bank_account acct{1, "Robby", 50};
```

And you can write functions on accounts:

```
deposit(Bank_account& account, unsigned long amount)
{
    account.balance += amount;
}
```

And then use them like so:

```
deposit(acct, 50);
```

This is well and good, but we can do better to ensure that `Bank_account` 
objects are used properly. We do this via a technique called data hiding, 
whereby we make some aspects of the representation invisible to most of our 
code, except for some special, privileged functions that can access them.

To do this for our `Bank_account` structure, we instead make it a `class`, 
and we make the member variables `private`. Then we access them through 
public *member functions*. Here's a start:

```
class Bank_account
{
private:
    unsigned long id_;
    std::string owner_;
    unsigned long balance_;
    
public:
    void deposit(unsigned long amount);
}
```

Things to note:

  - We added trailing underscores to the private variables; this is a 
  convention, nothing more.
   
  - `deposit` is a member function, which means that:
  
    1)  Whenever you call it, it has implicit access to a `Bank_account` 
    class value, and
    
    2)  It can see private members of that value.
    
The class declaration, like a struct declaration, goes in the header file. 
The implementation of the member function, like other functions, goes in a `
.cpp` file. It looks like this:

```
void Bank_account::deposit(unsigned long amount)
{
    balance_ += amount;
}
```

So `deposit` looks like it takes only one parameter—`amount`—when in fact it 
has a second, implicit `Bank_account` parameter. When we refer to `balance_` 
inside `deposit`, it refers to the `balance_` member variable of that 
implicit `Bank_account`. Note also that we prefix the name of `deposit` with 
`Bank_account::`, which is how we indicate that we are defining a member 
function.

To use the deposit function, we use the member function call syntax:

```
acct.deposit(5);
```

What we haven’t discussed yet is how a `Bank_account` object is created. We 
can’t do it the same way as we did with the struct, because the members of 
`Bank_account` are private, and the code trying to construct it isn’t allowed
to get at them directly. Instead, we add to the `Bank_account` class a 
special kind of member function called a *constructor*, which is called every
time a `Bank_account` object is created. The declaration now looks like this:

```
class Bank_account
{
public:
    Bank_account(unsigned long id, const std::string& owner);
    void deposit(unsigned long amount);
    
private:
    unsigned long id_;
    std::string owner_;
    unsigned long balance_;
}
```

I moved the public section before the private section because it’s more 
important. The first entry in the private section is the constructor, whose 
name is always the same as the name of the class. Note that it doesn’t have a
return type—this is because constructors don’t return anything. The 
constructor takes two parameters: an account ID and the account owner’s name,
but not a balance, since the initial balance will always be 0. The definition
of the constructor, in `src/Bank_account.cpp`, is responsible for doing the 
work of initializing the class. It looks like this:

```
Bank_account::Bank_account(unsigned long id, const std::string& owner)
    : id_{id}, owner_{owner}, balance_{0}
{ }
```

First, note that the constructor is also qualified by the class name 
(`Bank_account::`). However, the constructor is not an ordinary function, and
one difference is that *initialization* list that follows the parameters 
before the function body:

```
    : id_{id}, owner_{owner}, balance_{0}
```

This specifies how to initialize the member variables of the class—in this 
case, two are initialized from the parameters and one always to 0. There can 
be a body if there is more initialization work to do, but in this case there 
isn’t.

See `src/Bank_account.h` for the full declaration of the `Bank_account` 
class, including a number of member functions. Let’s look at two particular 
member functions from the class:

```
    const std::string& owner() const;
    void change_owner(const std::string& new_owner);
```

This is a pair of a *getter*, for getting at the value of a private member 
variable, and a *setter*, for changing it to something different. IMPORTANT: 
NOT ALL MEMBER VARIABLES HAVE SETTERS OR GETTERS. (Otherwise, what would be 
the point in making them private?) But in this case, we want to be able to 
both get and set the owner, so we provide both member functions. Also notice 
the trailing `const` on `owner()`—this means that when calling `owner` on a 
`Bank_account` object, the object is constant for the call. In other words, 
that `const` means that `owner()` doesn’t—in fact can’t—change the object on 
which it is called.

Next, let’s look at the member functions concerning the member ID. There is 
only one:

```
    unsigned long id() const;
```

This means that we can find out the account ID of an account, but we cannot 
change it, because there are no member functions for doing so.

Finally, let’s look at the member functions that involve the balance:

```
    unsigned long balance() const;
    void deposit(unsigned long);
    bool withdraw(unsigned long);
```

The first member function lets us observe the balance—it’s a getter. The 
other two functions let us change the balance, but in a controlled manner. 
Neither one is a setter, since neither one lets us choose the new balance 
directly. Instead, they restrict us to particular ways of changing the balance.
In more complex classes, the ways that the state can change will be even more
restricted.

### Stack and queue examples

As a second example of a class using data hiding, see `src/Stack.h` and 
`src/Stack.cpp`. These files declare and define a `Stack` class for 
representing LIFO stacks of `double`s. The `Stack` class encapsulates the 
Stack ADT, by providing us with members for manipulating the stack via its 
operations (push and pop), but without providing direct access to the 
representation. Privately, we can see that the stack is represented using a 
`std::vector<double>` to store its elements, but from outside the class, 
non-member functions cannot get at the vector directly. They can only access 
it via the public member functions that allow using it like a stack.

Files `src/Queue.h` and `src/Queue.cpp` give a third example of a class, 
similar to the second. They use the banker’s queue representation of a FIFO 
queue, though again the representation is hidden, and there’s no way to get 
at the underlying vectors except via the `enqueue` and `dequeue` member 
functions, which enforce the required queue discipline.

## Part 2: Union-find review

Last week we talked about the union-find structure for representing
disjoint sets of objects. We can directly express the Union-Find ADT as a 
class in C++. See `src/Union_find.h` for the interface, which declares a 
constructor for creating a union-find along with find and union operations. 
You will have to complete the implementation of this class for Homework 6.

## Part 3: A graph class

Homework 6 will also involve a graph algorithm; rather than have you design 
your own graph class, we design one in this lecture. You will then use this 
class when implementing your homework.

In `src/WU_graph.h` we declare a class `WU_graph` for representing a 
weighted, undirected graph. The class provides a constructor for creating a 
new graph with some given number of vertices, along with a member function 
for adding an edge. Then to observe the graph, there is a member function to 
find out all the vertices adjacent to a given vertex, and a member function 
to find out the weight of the edge, if any, between two given vertices. (Lack
of an edge is represented using the `double` value of positive infinity, 
which we name `NO_EDGE`. The variable `NO_EDGE` is a static member, which 
means that it is not duplicated for each instance of the `WU_graph` class, 
but just exists once, associated with the class.)

The private representation of the graph is as a vector of vectors of weights,
where the *v*th position of the *u*th vector gives the weight of the edge 
between *u* and *v*. (This is called an adjacency matrix representation.) 
In `src/WU_graph.cpp` you can find implementations of the member functions 
for observing a graph, and you can see how the implementations are written in
terms of the representation. If we wanted a different representation, we 
could change it and leave the signature of the class the same, which would 
allow client code to continue to work with graphs unchanged.

Below the graph class we define several functions that operate on graphs but 
are not members. First is `operator==`, which specifies how to compare two 
graphs for equality. Rather than being a member, `operator==` is a function 
that takes two `const WU_graph&`s. However, it needs to see the 
private representation of graphs in order to do the comparison. A friend 
declaration inside the `WU_graph` class gives it this permission:

```
    friend bool operator==(const WU_graph&, const WU_graph&);
```

The other free (non-member) functions work without privileged access to the 
graph class. See, for example, `get_all_edges(const WU_graph&)`, a function 
that collects all the edges in a graph and returns them in a vector. Below 
that are two graph algorithms we’ve seen before, Bellman-Ford and Dijkstra’s,
both of which are for computing single-source shortest paths. These functions
work on graphs without seeing their private representation at all, but rather
do all their work via the public member functions (the interface).
