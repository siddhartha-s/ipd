#include "Sbox_hash.h"

#include <random>

Sbox_hash::Sbox_hash()
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;
    for (auto& n : sbox_) n = dist(rng);
}


size_t Sbox_hash::operator()(const std::string& s) const
{
    size_t hash = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        hash ^= sbox_[(unsigned char)s[i]];
        hash *= 3;
    }

    return hash;
}

