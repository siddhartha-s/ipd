# Lecture 12: Intro to C++

For our first C++ program, see `src/hello.cpp`. Line by line:

At the beginning of each file, C++ requires that we tell it which parts
of the library that we’re going to use. In this case, we are going to do
I/O (printing to the screen and reading from the keyboard), and we’re
going to work with strings:

```c++
#include <iostream>
#include <string>
```

A C++ program is always a function called `main`, and this is how we
declare a function of no parameters in C++. Don’t worry about the `int`
for now:

```c++
int main()
{
    …
}
```

Here we print text to the screen. `std::cout` is the name of the
character output stream that we want to print to, and `<<` is the
stream insertion operator. It takes a stream and some data, and sends
the data to the stream.

```c++
    std::cout << "Please enter your name:\n> ";
```

When we want to use a variable in C++, we first need to declare it, which
tells C++ that we intend to use it, and what type of thing we intend to
store in it. In this case, we intend to use a string.

```c++
    std::string first_name;
```

Here we read from the standard input stream into `first_name`. C++ will
wait for the user to enter a string, which it will store in the
variable:

```c++
    std::cin >> first_name;
```

Finally, we print a message on the output stream:

```c++
    std::cout << "Hello, " << first_name << '\n';
```

Notice that if we enter more than one word as our name, C++ only gets
the first word. That’s because reading into a string using `>>` only
reads up to the first space. (We’ll see later how to overcome this
limitation.)

Let’s write a second program, which reads two pieces of data from the
user. See `src/age.cpp`.

## Integers and numbers

So far we’ve used two C++ types, `std::string` and `int`. Let’s add a
third type, `double`, which is used for fractional numbers. In C++, the
same operations do different things depending on the types involved.
Let’s consider some operations.

Suppose that `x` is declared to be an `int` or `double`. Then:

```c++
std::cin >> x;       // reads a number into x
std::cout << x;      // prints the number that’s in x
x = x + x;           // doubles the value of x
++x;                 // increments the value of x
```

But suppose `x` were a `std::string`. Then:

```c++
std::cin >> x;       // reads a word into x
std::cout << x;      // prints the string that’s in x
x = x + x;           // concatenates x to itself
++x;                 // ERROR
```

The type of a variable determines:

  - What operations are valid.
  - What they mean for that type.

## Arithmetic

C++ provides the usual arithmetic facilities. See `src/math.cpp` for an
example. Note that we `#include <cmath>`, which is where a lot of math
functions and constants live, in order to get `sqrt`.

Let’s write a program that reads the radius of a circle and then prints
its circumference. Because we are going to do I/O and math, we need to
include the headers for each:

```c++
#include <iostream>
#include <cmath>
```

Then we write the `main` function, with a sequence of statements as its
body. First we output the instructions to the user:

```c++
    std::cout << "Please enter the radius:\n> ";
```

Then we declare a variable to store the input, and read the input into
it:

```c++
    double r;
    std::cin >> r;
```

Finally, we compute the answer and print it out:

```c++
    std::cout << "The circumference is " << 2 * M_PI * r << '\n';
```

## Functions

As in BSL/ISL/DSSL, we will want to factor our programs into functions.
Here’s the definition of a simple C++ function:

```c++
// Converts a temperature from Fahrenheit to Celsius.
double f2c(double fahrenheit)
{
    return 5 * (fahrenheit - 32) / 9;
}
```

Let’s look at that piece by piece:

  - The first `double` is the return type of the function—what type of
    value it returns.

  - `f2c` is the name of the function.

  - `double fahrenheit` declares that the function takes one parameter
     with the given type and name.

  - As with `main`, we put the body of the function between {  }

  - We compute the result, and use the `return` keyword to exit the
    function with the computed result. (If we leave out `return` in a
    non-void function it’s an error [except for `main`, which is a
    special case].)

How can we use this function?

One way, what we probably ultimately want to do, to call the function
from `main`, or from another function, which eventually in turn is
called by `main`. You can see this in `src/f2c.cpp`, which defines a
`main` function that uses `f2c`.

When C++ is processing a .cpp file, it cannot see definitions from other
.cpp files unless we specifically tell it about them. So `src/f2c.cpp`
has a *declaration* of function `f2c`, which doesn’t give its
definition, but tells C++ that it exists and what its types are.

Another thing we may want to do, besides use the function in a program,
is to unit test it. To do this, we *don’t* write a `main`, but instead
create tests in a separate test file `test/functions.cpp`. (There’s
nothing special about the names.) In order for the tests to know about
the definition of `f2c`, we create a *header file* `src/functions.h`
that includes the declaration of `f2c`. We `#include` this file from
both `src/functions.cpp` (which ensures that the declarations match),
and from `test/functions.cpp` in order to see the function from
there.

## Structures

Like BSL and friends, C++ has structures. You declare one like this:

```c++
struct posn
{
    double x;
    double y;
}
```

This defines a new type `posn` that has two member variables, `x` and
`y`, both of type `double`.

To create a `posn`, we can declare a `posn` variable, giving the values
of the member variables in order, like so:

```c++
posn my_posn{3, 4};
```

Then `my_posn` is a variable of type `posn`. We can get the member
variables out using the dot operator:

```c++
CHECK_EQUAL(3, my_posn.x);
CHECK_EQUAL(4, my_posn.y);
```

Let’s write a function that computes the distance between two posns.
(See `src/posn.h` and `src/posn.cpp`, and `test/posn.cpp` for tests.)

Note that unlike BSL etc., C++ passes arguments by copying them. So
suppose you want to write a function that increments the `x` member of a
`posn`, leaving the `y` alone:

```c++
void shift(posn p)
{
    ++p.x;
}
```

The above function changes a `posn`, but it changes *a copy* of the
`posn` passed in by the caller. If we want the caller to see the change,
one thing we can do is return the changed posn:

```c++
posn shift(posn p)
{
    ++p.x;
    return p;
}
```

Then a caller might do

```c++
q = shift(q);
```

to shift `posn` `q`. There’s another solution that we’ll see in a bit.

## Vectors

Like DSSL, C++ also has vectors. See functions `sum` and `mean` in
`src/vectors.cpp` for examples of functions taking vectors. Note that as
with structs, vectors are passed by-copy, which means that a function
that attempts to modify its argument (`renorm0`) actually has no effect.
Instead, the function can return the modified vector (`renorm1`), or
using & declare that it takes the argument by reference (`renorm2`).

Some notes on C++ vectors:

  - They have a member function, .size(), that gives the number of
    elements.

  - There are two main ways to iterate. You can use a for-each loop like
    so:

    ```c++
    for (double f : v) ...
    ```

    In the above, `f` will get the value of each element of `v` in turn.

    Or you can use an explicitly counted for loop, like so:

    ```c++
    for (int i = 0; i < v.size(); ++i) ...
    ```

    This initializes `i` to 0, then repeats so long as its less than the
    size, incrementing `i` each time. Thus, `i` will range from 0 to the
    index of the last element, and to get the elements we use `v[i]`.

    Why do the renorm functions use the latter form?

  - Vector indexing `v[i]` is *UNCHECKED*, meaning if you index out of
    bounds, you don’t necessarily get an error. You might just get
    garbage. And if you assign out of bounds, you are writing garbage
    over some other data structure.

    For bounds checking, use `v.at(i)`. This will crash your program
    immediately if you attempt to index out of bounds.

