#include "f.hpp"

#include <iostream>

int main(int, const char*[])
{
    double d;

    std::cout << "Please enter a double, yo: ";
    std::cin >> d;

    std::cout << f(d) << '\n';
}
