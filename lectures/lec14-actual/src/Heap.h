#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace ipd {

template<typename T>
class Heap
{
public:
    void insert(const T&);
    T remove_min();
    const T& peek_min() const;

    bool empty() const;
    size_t size() const;

private:
    std::vector<T> heap_;
};

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

template<typename T>
bool Heap<T>::empty() const
{
    return heap_.empty();
}

template<typename T>
size_t Heap<T>::size() const
{
    return heap_.size();
}

template<typename T>
const T& Heap<T>::peek_min() const
{
    if (empty()) throw std::logic_error("Heap::get_min(): empty heap");

    return heap_.front();
}

template<typename T>
void Heap<T>::insert(const T& kd)
{
    size_t ix = size();
    heap_.push_back(kd);

    while (!is_root(ix) && heap_[ix] < heap_[parent(ix)]) {
        std::swap(heap_[ix], heap_[parent(ix)]);
        ix = parent(ix);
    }
}

namespace {

template<typename T>
size_t min_child(const std::vector<T>& heap, size_t ix)
{
    size_t result = ix;

    if (left_child(ix) < heap.size() &&
            heap[left_child(ix)] < heap[result])
        result = left_child(ix);

    if (right_child(ix) < heap.size() &&
            heap[right_child(ix)] < heap[result])
        result = right_child(ix);

    return result;
}

}

template<typename T>
T Heap<T>::remove_min()
{
    if (empty())
        throw std::logic_error("Heap::remove_min(): empty heap");

    std::swap(heap_.front(), heap_.back());
    T result = heap_.back();
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