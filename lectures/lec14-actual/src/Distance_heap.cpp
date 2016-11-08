#include "Distance_heap.h"

#include <stdexcept>
#include <utility>

namespace ipd {

namespace {

bool is_root(size_t ix)
{
    return ix == 0;
}

size_t parent(size_t ix)
{
    return (ix - 1) / 2;
}

size_t left_child(size_t ix)
{
    return 2 * ix + 1;
}

size_t right_child(size_t ix)
{
    return 2 * ix + 2;
}

}

bool Distance_heap::empty() const
{
    return heap_.empty();
}

size_t Distance_heap::size() const
{
    return heap_.size();
}

const known_distance& Distance_heap::peek_min() const
{
    if (empty()) throw std::logic_error("Distance_heap::get_min(): empty heap");

    return heap_.front();
}

void Distance_heap::insert(const known_distance& kd)
{
    size_t ix = size();
    heap_.push_back(kd);

    while (!is_root(ix) && heap_[ix].w < heap_[parent(ix)].w) {
        std::swap(heap_[ix], heap_[parent(ix)]);
        ix = parent(ix);
    }
}

namespace {

size_t min_child(const std::vector<known_distance>& heap, size_t ix)
{
    size_t result = ix;

    if (left_child(ix) < heap.size() &&
            heap[left_child(ix)].w < heap[result].w)
        result = left_child(ix);

    if (right_child(ix) < heap.size() &&
            heap[right_child(ix)].w < heap[result].w)
        result = right_child(ix);

    return result;
}

}

known_distance Distance_heap::remove_min()
{
    if (empty())
        throw std::logic_error("Distance_heap::remove_min(): empty heap");

    std::swap(heap_.front(), heap_.back());
    known_distance result = heap_.back();
    heap_.pop_back();

    size_t ix = 0;
    for (;;) {
        size_t next = min_child(heap_, ix);
        if (next == ix) break;

        std::swap(heap_[ix], heap_[next]);
        ix = next;
    }

    return result;
}

}