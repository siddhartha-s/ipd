#include "g.hpp"

// definition of f:
int f(int d)
{
    return max(g(8, max(3.0, static_cast<double>(d))), g1(7));
}
