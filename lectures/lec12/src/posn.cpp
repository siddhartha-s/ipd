#include "posn.h"
#include <cmath>

double distance(posn a, posn b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
