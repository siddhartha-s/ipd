#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

class Not_found : public std::logic_error
{
public:
    Not_found(const std::string& s) : logic_error("Not found: " + s) {}
};

template<typename T>
class Vec_hash
{
public:
    static const size_t default_size = 10000;

    Vec_hash(size_t size = default_size);

    void add(const std::string& key, const T& value);

    T& lookup(const std::string& key);

    const T& lookup(const std::string& key) const;

    bool member(const std::string& key) const;

    size_t collisions() const;

    size_t table_size() const;

    // This function really should be protected, but we made it public for
    // testing.
    virtual size_t hash(const std::string& s) const;

private:
    struct Pair
    {
        std::string key;
        T value;
    };
    std::vector<std::vector<Pair>> table_;

    size_t hash_size(const std::string& key) const;
};

template<typename T>
size_t Vec_hash<T>::hash(const std::string& s) const
{
    if (s.empty()) return 0;
    else return (unsigned char) s[0];
}

template<typename T>
size_t Vec_hash<T>::hash_size(const std::string& key) const
{
    return hash(key) % table_.size();
}


template<typename T>
Vec_hash<T>::Vec_hash(size_t size) : table_(size) {}


template<typename T>
void Vec_hash<T>::add(const std::string& key, const T& value)
{
    size_t hash_code = hash_size(key);
    for (Pair& p : table_[hash_code]) {
        if (p.key == key) {
            p.value = value;
            return;
        }
    }
    table_[hash_code].push_back({key, value});
}


template<typename T>
const T& Vec_hash<T>::lookup(const std::string& key) const
{
    size_t hash_code = hash_size(key);
    for (const Pair& p : table_[hash_code])
        if (p.key == key)
            return p.value;
    throw Not_found(key);
}


template<typename T>
T& Vec_hash<T>::lookup(const std::string& key)
{
    size_t hash_code = hash_size(key);
    for (Pair& p : table_[hash_code])
        if (p.key == key)
            return p.value;
    throw Not_found(key);
}

template<typename T>
bool Vec_hash<T>::member(const std::string& key) const
{
    size_t hash_code = hash_size(key);
    for (const Pair& p : table_[hash_code])
        if (p.key == key)
            return true;
    return false;
}


template<typename T>
size_t Vec_hash<T>::collisions() const
{
    size_t elements = 0;
    for (const std::vector<Pair>& v : table_) {
        elements += v.size();
    }
    size_t best_bucket_size = elements / table_.size();
    if (best_bucket_size * table_.size() != elements)
        best_bucket_size++;

    size_t collisions = 0;
    for (const std::vector<Pair>& v : table_) {
        if (v.size() > best_bucket_size)
            collisions += (v.size() - best_bucket_size);
    }
    return collisions;
}


template<typename T>
size_t Vec_hash<T>::table_size() const
{
    return table_.size();
}

void hash_trial(std::string name, Vec_hash<size_t>& h);

const std::vector<std::string>& get_hamlet();
