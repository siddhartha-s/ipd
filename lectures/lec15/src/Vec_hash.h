#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

// Thrown by Vec_hash<T>::lookup when the key isn't found.
class Not_found : public std::logic_error
{
public:
    // Constructs a `Not_found` exception with the given key name.
    Not_found(const std::string& s) : logic_error("Not found: " + s) {}
};

// A separate-chained hash table with std::strings for keys and type
// parameter `T` for values.
template<typename T>
class Vec_hash
{
public:
    // The default number of buckets.
    static const size_t default_size = 10000;

    // Constructs a new hash table, optionally specifying the number of buckets.
    Vec_hash(size_t size = default_size);

    // Inserts a key-value association into the hash table.
    void add(const std::string& key, const T& value);

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    T& lookup(const std::string& key);

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    const T& lookup(const std::string& key) const;

    // Returns whether the given key is found in the hash table.
    bool member(const std::string& key) const;

    // Returns the number of buckets.
    size_t table_size() const;

    // Hashes a string to a 64-bit hash code.
    //
    // This function really should be protected, but we made it public for
    // testing.
    virtual size_t hash(const std::string& s) const;

private:
    struct Entry
    {
        std::string key;
        T value;
    };
    std::vector<std::vector<Entry>> table_;

    // Hashes the given string and mods by the table size. This gives the
    // index into the table.
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
    for (Entry& p : table_[hash_code]) {
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
    for (const Entry& p : table_[hash_code])
        if (p.key == key)
            return p.value;
    throw Not_found(key);
}


template<typename T>
T& Vec_hash<T>::lookup(const std::string& key)
{
    size_t hash_code = hash_size(key);
    for (Entry& p : table_[hash_code])
        if (p.key == key)
            return p.value;
    throw Not_found(key);
}

template<typename T>
bool Vec_hash<T>::member(const std::string& key) const
{
    size_t hash_code = hash_size(key);
    for (const Entry& p : table_[hash_code])
        if (p.key == key)
            return true;
    return false;
}


template<typename T>
size_t Vec_hash<T>::table_size() const
{
    return table_.size();
}

