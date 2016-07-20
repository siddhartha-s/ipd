#include "g.hpp"
#include <cmath>

const long MY_CONST = 9;

int g(int n, double d)
{
    return std::floor(n * d);
}

int g1(int n)
{
    return g(n, 2.0);
}
