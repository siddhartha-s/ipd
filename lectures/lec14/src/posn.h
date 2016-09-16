#pragma once

#include <cmath>

template <typename T>
struct posn
{
    T x;
    T y;
};

template <typename T>
double distance(const posn<T>& p, const posn<T>& q)
{
    T dx = p.x - q.x;
    T dy = p.y - q.y;
    return sqrt(dx*dx + dy*dy);
}