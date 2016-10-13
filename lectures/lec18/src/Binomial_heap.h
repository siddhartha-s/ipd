#pragma once

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

namespace ipd {

template<typename T>
class Binomial_heap
{
public:
    bool empty() const;
    size_t size() const;

    void add(const T&);
    const T& get_min() const;
    void remove_min();

    void merge(Binomial_heap&);

private:
    struct node_;
    using tree_ = std::unique_ptr<node_>;
    using row_  = std::vector<tree_>;

    row_   roots_;
    size_t size_ = 0;

    size_t find_min_index_() const;

    static tree_ merge_trees_(tree_, tree_);
    static row_ merge_rows_(row_&, row_&);
};

template<typename T>
struct Binomial_heap<T>::node_
{
    node_(const T& value) : data(value) {}

    T    data;
    row_ children;
};

template<typename T>
bool Binomial_heap<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
size_t Binomial_heap<T>::size() const
{
    return size_;
}

template<typename T>
void Binomial_heap<T>::add(const T& value)
{
    Binomial_heap singleton;
    singleton.size_ = 1;
    singleton.roots_.push_back(std::make_unique<node_>(value));
    merge(singleton);
}

template<typename T>
const T& Binomial_heap<T>::get_min() const
{
    return roots_[find_min_index_()]->data;
}

template<typename T>
void Binomial_heap<T>::remove_min()
{
    tree_ victim = std::move(roots_[find_min_index_()]);
    row_  result = merge_rows_(roots_, victim->children);
    std::swap(result, roots_);

    --size_;
}

template<typename T>
size_t Binomial_heap<T>::find_min_index_() const
{
    size_t best = 0;

    for (size_t i = 1; i < roots_.size(); ++i) {
        if (roots_[best] == nullptr ||
            (roots_[i] != nullptr &&
             roots_[i]->data < roots_[best]->data)) {
            best = i;
        }
    }

    return best;
}

template<typename T>
void Binomial_heap<T>::merge(Binomial_heap& other)
{
    row_ result = merge_rows_(roots_, other.roots_);
    std::swap(result, roots_);
    size_ += other.size_;
    other.size_ = 0;
}

template<typename T>
auto Binomial_heap<T>::merge_trees_(tree_ a, tree_ b) -> tree_
{
    if (a->data < b->data) {
        a->children.push_back(std::move(b));
        return a;
    } else {
        b->children.push_back(std::move(a));
        return b;
    }
}

template<typename T>
auto Binomial_heap<T>::merge_rows_(row_& a, row_& b) -> row_
{
    std::vector<tree_> carry;
    row_               result;

    size_t limit = std::max(a.size(), b.size());

    for (size_t i = 0; i < limit; ++i) {
        if (i < a.size() && a[i] != nullptr) carry.push_back(std::move(a[i]));
        if (i < b.size() && b[i] != nullptr) carry.push_back(std::move(b[i]));

        switch (carry.size()) {
            case 0:
                result.push_back(nullptr);
                carry.clear();
                break;

            case 1:
                result.push_back(std::move(carry[0]));
                carry.clear();
                break;

            case 2: {
                result.push_back(nullptr);
                tree_ merged = merge_trees_(std::move(carry[0]),
                                            std::move(carry[1]));
                carry.clear();
                carry.push_back(std::move(merged));
                break;
            }

            case 3: {
                result.push_back(std::move(carry[2]));
                tree_ merged = merge_trees_(std::move(carry[0]),
                                            std::move(carry[1]));
                carry.clear();
                carry.push_back(std::move(merged));
                break;
            }

            default:
                assert(false);
        }
    }

    if (!carry.empty())
        result.push_back(std::move(carry[0]));

    a.clear();
    b.clear();

    return result;
}

}