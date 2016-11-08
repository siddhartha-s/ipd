#pragma once
#include "Vec_hash.h"
#include <array>
#include <random>


template<typename T>
class Identity_hash : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    using Vec_hash<T>::Vec_hash;
};

template<typename T>
size_t Identity_hash<T>::hash(const std::string& s) const
{
    if (s.length() != 8) return 0;
    size_t      ret = 0;
    for (size_t i   = 0; i < s.length(); i++) {
        ret |= ((size_t)(unsigned char) s[i]) << (i * 8);
    }
    return ret;
};

