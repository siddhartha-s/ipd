#include "Stack.h"

#include <stdexcept>

void Stack::push(element_t elt)
{
    elements_.push_back(elt);
}

Stack::element_t Stack::pop()
{
    if (elements_.empty())
        throw std::logic_error{"Stack::pop(): empty stack"};

    element_t result = elements_.back();
    elements_.pop_back();
    return result;
}

size_t Stack::size() const
{
    return elements_.size();
}

