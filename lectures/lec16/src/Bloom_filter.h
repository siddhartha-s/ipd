#pragma once

#include "Sbox_hash.h"

#include <string>
#include <vector>

// An approximate set of strings. May return false positives, but not false
// negatives. A 1% false positive rate may be achieved using 9.6 bits per
// entry, with each additional 4.8 bits per entry reducing the false positive
// rate by a factor of 10.
class Bloom_filter
{
public:
    // Constructs a new filter of size `nbits`, using `nfunctions` different
    // hash functions.
    explicit Bloom_filter(size_t nbits, size_t nfunctions = 3);

    // Inserts the string into the filter.
    void insert(const std::string&);

    // Checks whether the string is in the filter. May return false positives.
    bool check(const std::string&);

private:
    size_t nbits_;
    std::vector<Sbox_hash> functions_;
    std::vector<bool> bits_;
};