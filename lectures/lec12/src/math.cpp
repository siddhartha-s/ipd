#include <iostream>
#include <cmath>

// Strategy: function composition
int main()
{
    std::cout << "Please enter a floating-point number:\n> ";

    double f;
    std::cin >> f;

    std::cout <<   "f     == " << f
              << "\nf + 1 == " << f + 1
              << "\n2f    == " << 2 * f
              << "\n3f    == " << 3 * f
              << "\nf²    == " << f * f
              << "\n√f    == " << sqrt(f) << '\n';
}
