# Lecture 20: Inheritance and Virtual Functions

## Inheritance

C++ provides a mechanism for defining families of classes that are similar in
some ways and different in others. For example, suppose we want classes to 
represent different kinds of pet animal, which have operations like sleeping
and playing. We can combine their common behavior into a *base class* and then
*derive* the specific animal classes from that. 

Let’s call the base class `Animal`. We’ll initially define it as follows:

```c++
class Animal
{
public:
    Animal(const std::string& name, unsigned int weight);

    void eat(unsigned int amount);
    void play();

    const std::string& get_name() const { return name_; }
    unsigned int get_weight() const { return weight_; }

private:
    std::string name_;
    unsigned int weight_;
};
```

That is, every animal has a name and a weight, and has operations to eat and 
to play. Here are the implementations of the operations:

```c++
Animal::Animal(const std::string& name, unsigned int weight)
        : name_{name}, weight_{weight}
{ }

void Animal::eat(unsigned int amount)
{
    weight_ += amount;
}

void Animal::play()
{
    std::cout << get_name() << " plays.\n";
}
```

We then *derive* other classes from the `Animal` class. Deriving makes the 
new class like the old class, but then lets us add to or change it. Here’s 
`Cat`:

```c++
class Cat : public Animal
{
public:
    Cat(const std::string& name);
    void speak();
};

Cat::Cat(const std::string& name)
        : Animal{name, 10}
{ }

void Cat::speak()
{
    std::cout << get_name() << " says meow!\n";
}
```

The notation `: public Animal` means that `Cat` starts out as a copy of 
`Animal`, with all of `Animal`’s public members public in `Cat` as well. We 
then define a constructor, which takes a name and passes it on to the base 
class’s constructor, along with a fixed weight of 10.

`Cat` also has a new member function, `speak`, that `Animal` does not have.

We also define `Dog` by deriving from `Animal`, but unlike `Cat`, `Dog`’s 
playing behavior is a bit more complicated. In particular, each `Dog` has 
some number of bones, and each time we play the dog gets a new bone. And then
when the dog speaks, it says “woof” repeatedly, once for each of its bones.
Here’s the definition of the `Dog` class:

```c++
class Dog : public Animal
{
public:
    Dog(const std::string& name);
    void speak();
    void play();

private:
    unsigned int nbones_;
};

Dog::Dog(const std::string& name)
        : Animal{name, 100}
        , nbones_{1}
{ }

void Dog::speak()
{
    std::cout << get_name() << " says";
    for (int i = 0; i < nbones_; ++i)
        std::cout << " woof";
    std::cout << "!\n";
}

void Dog::play()
{
    Animal::play();
    ++nbones_;
}
```

As you can see, `Dog` adds a member variable `nbones_` to `Animal`, adds a 
member function `speak`, and replaces the generic `play` function with a 
`Dog`-specific `play` function. The `Dog::play` function delegates to 
`Animal::play` to do the playing and also increments `nbones_`.

Now we can write a program involving cats and dogs, and their behaviors:

```c++
    Dog willie("Willie");
    Cat vinny("Vinny");
    Cat francie("Francie");
    
    willie.play();
    vinny.play();
    francie.eat(2);
```

Output:

```c++
Willie plays.
Vinny plays.
```

## Polymorphism

An additional feature of inheritance is a form of polymorphism. Polymorphism 
is when the same variable (or more abstractly, entity) can come in different 
forms. In this case, the polymorphism is that a reference or pointer whose 
type says it’s of the base class can actually refer to an object of any 
derived class. Here we have a function that takes an `Animal` reference:

```c++
void play_twice(Animal& an)
{
    an.play();
    an.play();
}
```

We can use the reference according to the public interface of `Animal`, but 
at run time, the actual object the reference points to is allowed to be any 
derived class of `Animal`:

```c++
    play_twice(willie);
    play_twice(vinny);
```

There are two problems with this, however:

  - Because we made `speak` different for the two derived classes and didn’t 
  include it in the base class, we can’t call it via a base class reference.
  
  - When we call a function on the base class reference, it uses the base 
  class version of the function. That is, when we say `an.play()`, it uses 
  the `Animal::play` member function rather than `Dog::play`, even when `an` 
  is a `Dog`. In particular, the `nbones_` member variable won’t be 
  incremented when we call `play_twice` on the `Dog` `willie`, even though 
  `Dog::play` increments `nbones_`.
  
The solution to these problems is a *virtual function*. Declaring a function 
`virtual` in the base class means that we expect the derived classes to 
`override` it, and we want to get the overridden derived class behavior, even
with working with a base class reference. To do this, we change the 
declaration of `Animal::play` to be virtual:

```c++
class Animal
{
public:
    …
    virtual void play();
    …
};
```

Then in `Dog`, we redefine `play` and indicate that we are overriding the 
base definition of `play`:

```c++
class Dog
{
    …
    void play() override;
    …
};
```

Now when we use a `Dog` object via an `Animal&` reference, it uses 
`Dog::play` rather than `Animal::play`.

This is polymorphic because the `Animal&` does not necessarily refer to an 
`Animal` object, but different kinds of derived classes with potentially 
different behaviors.

## Pure virtual functions

We did not define a `speak` function in the `Animal` class, since that 
function is different for the derived classes, and so there was no sharing to
be had. But this prevents us from calling `speak` on an `Animal&`, even if 
the object referred to is actually a `Cat` or `Dog`. It has to be this way 
because there’s no guarantee of every derived class of `Animal` will define 
`speak`. We can fix this by declaring in `Animal` that every derived class of
`Animal` *must* define `speak`. We do this by declaring `speak` to be *pure 
virtual*:
 
```c++
class Animal
{
    …
    virtual void speak() = 0;
    …
};
```

The `= 0` means that `Animal` will not define `speak`, but that its derived 
classes must. In particular, `Animal` is considered an *abstract class* 
because its definition is incomplete, and this prevents `Animal` objects from
being instantiated. However, we define (override) `speak` in `Cat` and `Dog`,
and this makes those classes *concrete* and instantiable:

```c++
class Cat : public Animal
{
    …
    void speak() override;
    …
};

class Dog : public Animal
{
    …
    void speak() override;
    …
};
```

The definitions of `Cat::speak` and `Dog::speak` remain unchanged.

See `test/animals.cpp` for the full code.
