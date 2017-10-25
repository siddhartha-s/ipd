#include <iostream>
#include <string>

// Strategy: sequential composition
int main()
{
    std::cout << "Please enter your name:\n> ";

    std::string first_name;
    std::cin >> first_name;

    std::cout << "Hello, " << first_name << '\n';
}
