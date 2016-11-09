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

// Thrown by Vec_hash<T>::add when the table is full
class Full : public std::logic_error
{
public:
    Full(const std::string& s) : logic_error("Table overflowed: " + s) {}
};


// A separate-chained hash table with std::strings for keys and type
// parameter `T` for values.
template<typename T>
class Vec_open_hash
{
public:
    // The default number of buckets.
    static const size_t default_size = 10000;

    // Constructs a new hash table, optionally specifying the number of buckets.
    Vec_open_hash(size_t size = default_size);

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

    // Diagnostic function for measuring the number of collisions.
    size_t collisions() const;

    // Returns the number of buckets.
    size_t table_size() const;

    // Hashes a string to a 64-bit hash code.
    //
    // This function really should be protected, but we made it public for
    // testing.
    virtual size_t hash(const std::string& s) const;

private:
    struct Pair
    {
        std::string key;
        T           value;
        bool        valid;
    };
    std::vector<Pair>   table_;

    // Hashes the given string and mods by the table size. This gives the
    // index into the table.
    size_t get_index(const std::string& key) const;

    void double_size();

    void add_no_double(const std::string& key, const T& value);

};

template<typename T>
size_t Vec_open_hash<T>::hash(const std::string& s) const
{
    if (s.empty()) return 0;
    else return (unsigned char) s[0];
}

template<typename T>
Vec_open_hash<T>::Vec_open_hash(size_t size) : table_(size)
{
    for (Pair& p : table_) {
        p.valid = false;
    }
}

template<typename T>
void Vec_open_hash<T>::double_size()
{
    std::vector<Pair> old_table = move(table_);
    std::vector<Pair> new_table(old_table.size()*2);
    for (Pair& p : new_table) {
        p.valid = false;
    }
    table_ = move(new_table);
    for (Pair& p : old_table) {
        if (p.valid)
            add_no_double(p.key,p.value);
    }
}

template<typename T>
size_t Vec_open_hash<T>::get_index(const std::string& key) const
{
    size_t start = hash(key) % table_.size();
    size_t index = start;
    while (true) {
        const Pair& p = table_[index];
        if (p.key == key) return index;
        if (!p.valid) return index;
        if (p.valid) {
            index = (index + 1) % table_.size();
            if (index == start) {
                throw Full(key);
            }
        }
    }
}

template<typename T>
void Vec_open_hash<T>::add_no_double(const std::string& key, const T& value)
{
    size_t index = get_index(key);
    Pair& p = table_[index];
    p.key   = key;
    p.value = value;
    p.valid = true;
    return;
}


template<typename T>
void Vec_open_hash<T>::add(const std::string& key, const T& value)
{
    try {
        add_no_double(key, value);
    } catch (Full e) {
        double_size();
        add_no_double(key, value);
    }
}

template<typename T>
const T& Vec_open_hash<T>::lookup(const std::string& key) const
{
    try {
        const Pair& p = table_[get_index(key)];
        if (p.valid) return p.value;
        throw Not_found(key);
    } catch (Full e) {
        throw Not_found(key);
    }
}


template<typename T>
T& Vec_open_hash<T>::lookup(const std::string& key)
{
    try {
        Pair& p = table_[get_index(key)];
        if (p.valid) return p.value;
        throw Not_found(key);
    } catch (Full e) {
        throw Not_found(key);
    }
}

template<typename T>
bool Vec_open_hash<T>::member(const std::string& key) const
{
    try {
        const Pair& p = table_[get_index(key)];
        return p.valid;
    } catch (Full e) {
        return false;
    }
}

template<typename T>
size_t Vec_open_hash<T>::table_size() const
{
    return table_.size();
}
