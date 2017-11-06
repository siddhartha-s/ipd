#pragma once

#include "WU_graph.h"

namespace ipd
{

// This class is like `Distance_heap`, but it maintains a map from each
// key that it stores to its unique location in the heap. This means that:
//
//  - It cannot contain the same key more than once.
//  - Instead of an insert operation, it offer an update operation that will
//    move the key within the heap if it's already there.
//
// Absence from the heap and a distance of infinity are considered equivalent.
// In particular, setting the distance of a key to infinity will remove it
// from the heap.
class Dist_heap_map
{
public:
    using key = size_t;
    using dist = double;

    struct known_distance
    {
        key v;
        dist w;
    };

    // Updates the weight of a particular key. Each key is mapped to a
    // unique weight and thus this may update the weight of an already finite
    // key.
    void update(const known_distance&);
    known_distance remove_min();
    const known_distance& peek_min() const;

    // Returns the weight associated with a particular key; infinity if not
    // found.
    dist operator[](key) const;

    bool empty() const;
    size_t size() const;

private:
    // The actual heap, pairing keys with distances.
    std::vector<known_distance> heap_;
    // The index map, giving the index of each key in `heap_`.
    std::vector<size_t> indices_;

    // Inserts the given pairing.
    // PRECONDITION: the key is absent.
    void insert_(const known_distance&);
    // Increases the key of the given pairing.
    // PRECONDITION: the key is present at `ix`, mapped to a lesser distance.
    void increase_key_(size_t ix, const known_distance&);
    // Decreases the key of the given pairing.
    // PRECONDITION: the key is present at `ix`, mapped to a greater distance.
    void decrease_key_(size_t ix, const known_distance&);
    // Removes the pairing at the given index.
    // PRECONDITION: the index is in bounds.
    void remove_(size_t ix);

    // Moves the pairing of the given index down, if necessary.
    void percolate_down_(size_t);
    // Moves the pairing of the given index up, if necessary.
    // PRECONDITION: index is in bounds.
    void bubble_up_(size_t);

    // Finds the index of `key` in `heap_`, or size_t max otherwise.
    size_t index_of_(key) const;
    // Finds the index of the minimum element of the given index or either
    // of its children.
    size_t min_child_(size_t) const;

    // Removes the last element from the heap, updating indices_.
    void pop_back_();
    // Swaps two elements in the heap, updating indices_.
    void swap_(size_t, size_t);
    // Adds an element to the back of the heap, updating indices_.
    void push_back_(const known_distance& kd);
};

}
