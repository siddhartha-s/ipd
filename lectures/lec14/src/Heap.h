#pragma once

#include "WU_graph.h"

namespace ipd
{

template <typename Element>
class Heap
{
public:
    void insert(const Element&);
    Element remove_min();
    const Element& peek_min() const;

    bool empty() const;
    size_t size() const;

private:
    std::vector<Element> heap_;
};

namespace heap_helpers {

inline bool is_root(size_t ix)
{
    return ix == 0;
}

inline size_t parent(size_t ix)
{
    return (ix - 1) / 2;
}

inline size_t left_child(size_t ix)
{
    return 2 * ix + 1;
}

inline size_t right_child(size_t ix)
{
    return 2 * ix + 2;
}

}

template <typename Element>
void Heap<Element>::insert(const Element& elt)
{
    using namespace heap_helpers;

    size_t ix = heap_.size();
    heap_.push_back(elt);

    while (!is_root(ix) && heap_[ix] < heap_[parent(ix)]) {
        std::swap(heap_[ix], heap_[parent(ix)]);
        ix = parent(ix);
    }
}

template <typename Element>
static size_t min_child(const std::vector<Element>& heap, size_t ix)
{
    using namespace heap_helpers;

    size_t result = ix;

    if (left_child(ix) < heap.size() && heap[left_child(ix)] < heap[result])
        result = left_child(ix);

    if (right_child(ix) < heap.size() && heap[right_child(ix)] < heap[result])
        result = right_child(ix);

    return result;
}

template <typename Element>
Element Heap<Element>::remove_min()
{
    using namespace heap_helpers;

    if (empty()) throw std::logic_error("Edge_heap::remove_min(): empty heap");

    std::swap(heap_.front(), heap_.back());
    Element result = heap_.back();
    heap_.pop_back();

    size_t ix = 0;
    for (;;) {
        size_t next = min_child(heap_, ix);
        if (next == ix) break;

        std::swap(heap_[next], heap_[ix]);
        ix = next;
    }

    return result;
}

template <typename Element>
const Element& Heap<Element>::peek_min() const
{
    if (empty()) throw std::logic_error("Edge_heap::get_min(): empty heap");

    return heap_[0];
}

template <typename Element>
bool Heap<Element>::empty() const
{
    return heap_.empty();
}

template <typename Element>
size_t Heap<Element>::size() const
{
    return heap_.size();
}


}
