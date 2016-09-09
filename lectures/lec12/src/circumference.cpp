#include <iostream>
#include <cmath>

int main()
{
    std::cout << "Please enter the radius:\n> ";

    double r;
    std::cin >> r;

    std::cout << "The circumference is " << 2 * M_PI * r << '\n';
}
