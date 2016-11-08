#pragma once

#include <cmath>
#include <string>

namespace ipd {

struct int_posn
{
    int x;
    int y;
};

struct double_posn
{
    double x;
    double y;
};

template<typename T>
struct posn
{
    T x;
    T y;
};

template<typename T>
double distance(const posn<T>& p, const posn<T>& q)
{
    T dx = p.x - q.x;
    T dy = p.y - q.y;

    return sqrt(dx*dx + dy*dy);
}

template<>
double distance(const posn<std::string>& p, const posn<std::string>& q)
{
    return 0;
}

}