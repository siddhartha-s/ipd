#include "Dist_heap_map.h"

#include <limits>
#include <utility>

namespace ipd
{

static bool is_root(size_t ix)
{
    return ix == 0;
}

static size_t parent(size_t ix)
{
    return (ix - 1) / 2;
}

static size_t left_child(size_t ix)
{
    return 2 * ix + 1;
}

static size_t right_child(size_t ix)
{
    return 2 * ix + 2;
}

size_t constexpr BAD_INDEX()
{
    return std::numeric_limits<size_t>::max();
}

Dist_heap_map::dist constexpr INFINITY()
{
    return std::numeric_limits<Dist_heap_map::dist>::infinity();
}

void Dist_heap_map::update(const known_distance& kd)
{
    size_t ix = index_of_(kd.v);

    if (ix == BAD_INDEX()) {
        if (kd.w != INFINITY()) insert_(kd);
    }
    else if (kd.w == INFINITY()) remove_(ix);
    else if (kd.w < heap_[ix].w) decrease_key_(ix, kd);
    else if (kd.w > heap_[ix].w) increase_key_(ix, kd);
}

Dist_heap_map::known_distance Dist_heap_map::remove_min()
{
    known_distance result = peek_min();
    remove_(0);
    return result;
}

const Dist_heap_map::known_distance& Dist_heap_map::peek_min() const
{
    if (empty()) throw std::logic_error("Edge_heap::peek_min(): empty heap");

    return heap_[0];
}

Dist_heap_map::dist
Dist_heap_map::operator[](key v) const
{
    size_t index = index_of_(v);

    if (index == BAD_INDEX()) return INFINITY();
    else return heap_[index].w;
}

bool Dist_heap_map::empty() const
{
    return heap_.empty();
}

size_t Dist_heap_map::size() const
{
    return heap_.size();
}

void Dist_heap_map::insert_(const known_distance& kd)
{
    push_back_(kd);
    bubble_up_(size() - 1);
}

void Dist_heap_map::increase_key_(size_t ix, const known_distance& kd)
{
    heap_[ix].w = kd.w;
    percolate_down_(ix);
}

void Dist_heap_map::decrease_key_(size_t ix, const known_distance& kd)
{
    heap_[ix].w = kd.w;
    bubble_up_(ix);
}

void Dist_heap_map::remove_(size_t ix)
{
    swap_(ix, size() - 1);
    pop_back_();
    percolate_down_(ix);
}

size_t Dist_heap_map::index_of_(key v) const
{
    if (v < indices_.size()) {
        return indices_[v];
    } else {
        return BAD_INDEX();
    }
}

size_t Dist_heap_map::min_child_(size_t ix) const
{
    size_t result = ix;

    if (left_child(ix) < heap_.size() &&
            heap_[left_child(ix)].w < heap_[result].w)
        result = left_child(ix);

    if (right_child(ix) < heap_.size() &&
            heap_[right_child(ix)].w < heap_[result].w)
        result = right_child(ix);

    return result;
}

void Dist_heap_map::swap_(size_t i, size_t j)
{
    key v = heap_[i].v;
    key w = heap_[j].v;

    std::swap(heap_[i], heap_[j]);
    std::swap(indices_[v], indices_[w]);
}

void Dist_heap_map::percolate_down_(size_t ix)
{
    for (;;) {
        size_t next = min_child_(ix);
        if (next == ix) break;

        swap_(next, ix);
        ix = next;
    }
}

void Dist_heap_map::bubble_up_(size_t ix)
{
    while (!is_root(ix) && heap_[ix].w < heap_[parent(ix)].w) {
        swap_(ix, parent(ix));
        ix = parent(ix);
    }
}

void Dist_heap_map::pop_back_()
{
    key v = heap_.back().v;
    heap_.pop_back();

    indices_[v] = BAD_INDEX();

// This destroys our amortized complexity, and it isn't necessary:
    /*
    while (!indices_.empty() && indices_.back() == BAD_INDEX())
        indices_.pop_back();
    */
}

void Dist_heap_map::push_back_(const known_distance& kd)
{
    while (kd.v >= indices_.size())
        indices_.push_back(BAD_INDEX());
    indices_[kd.v] = heap_.size();

    heap_.push_back(kd);
}

}

