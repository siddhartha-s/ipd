#include <string>
#include <iostream>

class Animal
{
public:
    Animal(const std::string& name, unsigned int weight);

    void eat(unsigned int amount);
    virtual void speak() = 0;
    virtual void play();

    const std::string& get_name() const;
    unsigned int get_weight() const;

private:
    std::string name_;
    unsigned int weight_;
};

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

const std::string& Animal::get_name() const
{
    return name_;
}

unsigned int Animal::get_weight() const
{
    return weight_;
}


class Dog : public Animal
{
public:
    Dog(const std::string& name);
    void speak() override;
    void play() override;

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


class Cat : public Animal
{
public:
    Cat(const std::string& name);
    void speak() override;
};

Cat::Cat(const std::string& name)
        : Animal{name, 10}
{ }

void Cat::speak()
{
    std::cout << get_name() << " says meow!\n";
}

int main()
{
    Cat vinny("Vinny");
    Dog willie("Willie");

    willie.speak();
    willie.play();
    willie.speak();

    Animal& someone = vinny;
    someone.play();
}