#include <vector>

// Sums the elements of a vector.
double sum(std::vector<double>);
// Computes the mean of the elements of a vector.
double mean(std::vector<double>);

// Adjusts each element of a vector to be its displacement from the mean.
void renorm0(std::vector<double>);
// Adjusts each element of a vector to be its displacement from the mean.
std::vector<double> renorm1(std::vector<double>);
// Adjusts each element of a vector to be its displacement from the mean.
void renorm2(std::vector<double>&);
