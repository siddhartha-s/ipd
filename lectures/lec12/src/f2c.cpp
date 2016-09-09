#include <iostream>

double f2c(double);

int main()
{
    std::cout << "Enter the temperature in °F:\n> ";

    double f;
    std::cin >> f;

    std::cout << "The temperature equals " << f2c(f) << " °C\n";
}
