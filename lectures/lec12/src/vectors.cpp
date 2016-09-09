#include "vectors.h"
#include <vector>

// Sums the elements of a vector.
double sum(std::vector<double> v)
{
    double result = 0;

    for (double f : v)
        result += f;

    return result;
}

// Computes the mean of the elements of a vector.
// ASSUMPTION: v.size() != 0
double mean(std::vector<double> v)
{
    return sum(v) / v.size();
}

// Renormalizes a vector by changing every element to its displacement
// from the mean. (BUGGY!)
void renorm0(std::vector<double> v)
{
    double m = mean(v);

    for (int i = 0; i < v.size(); ++i)
        v[i] = v[i] - m;
}

// Renormalizes a vector by changing every element to its displacement
// from the mean.
std::vector<double> renorm1(std::vector<double> v)
{
    double m = mean(v);

    for (int i = 0; i < v.size(); ++i)
        v[i] = v[i] - m;

    return v;
}

// Renormalizes a vector by changing every element to its displacement
// from the mean.
void renorm2(std::vector<double>& v)
{
    double m = mean(v);

    for (int i = 0; i < v.size(); ++i)
        v[i] = v[i] - m;
}
