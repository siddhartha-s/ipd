#include <iostream>
#include <string>

// Strategy: sequential composition
int main()
{
    std::cout << "Please enter your first name and age:\n> ";
    std::string first_name;
    int age;
    std::cin >> first_name >> age;

    std::cout << "Hello, " << first_name << ", age " << age << '\n';
}
