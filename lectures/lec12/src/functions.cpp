#include "functions.h"

// Converts a temperature from Fahrenheit to Celsius.
double f2c(double fahrenheit)
{
    return 5 * (fahrenheit - 32) / 9;
}

// Converts a temperature from Celsius to Fahrenheit.
double c2f(double celsius)
{
    return 9 * celsius / 5 + 32;
}

// Computes the area of a rectangle.
double rectangle_area(double w, double h)
{
    return w * h;
}
