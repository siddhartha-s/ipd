#pragma once

extern const long MY_CONST;

int g(int, double);
int g1(int);

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}
