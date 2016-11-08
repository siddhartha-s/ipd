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


template<typename T>
class One_byte : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    using Vec_hash<T>::Vec_hash;
};

template<typename T>
size_t One_byte<T>::hash(const std::string& s) const
{
    size_t      ret = 0;
    for (size_t i   = 0; i < s.length(); i++) {
        ret ^= (unsigned char) s[i];
    }
    return ret;
};

template<typename T>
class Eight_bytes : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    using Vec_hash<T>::Vec_hash;
};

template<typename T>
size_t Eight_bytes<T>::hash(const std::string& s) const
{
    size_t      ret = 0;
    for (size_t i   = 0; i < s.length(); i += 8) {
        for (size_t j = 0; j < 8; j++) {
            ret ^= (unsigned char) s[i + j] << j * 8;
        }
    }
    return ret;
};


template<typename T>
class Simple_mix : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    using Vec_hash<T>::Vec_hash;
};

template<typename T>
size_t Simple_mix<T>::hash(const std::string& s) const
{
    size_t      ret = 0;
    for (size_t i   = 0; i < s.length(); i++) {
        ret ^= (unsigned char) s[i];
        ret *= 3;
    }
    return ret;
};


template<typename T>
class Sbox_hash : public Vec_hash<T>
{
public:
    virtual size_t hash(const std::string& s) const override;

    Sbox_hash(size_t = Vec_hash<T>::default_size);

private:
    std::array<size_t, 256> sbox_;
};

template<typename T>
Sbox_hash<T>::Sbox_hash(size_t size) : Vec_hash<T>(size)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;
    for (auto& n : sbox_) n = dist(rng);
}


template<typename T>
size_t Sbox_hash<T>::hash(const std::string& s) const
{
    size_t hash = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        hash ^= sbox_[(unsigned char)s[i]];
        hash *= 3;
    }

    return hash;
}