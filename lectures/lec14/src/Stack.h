#pragma once

#include <cstddef>
#include <vector>

// A stack of doubles.
class Stack
{
public:
    using element_t = double;

    // Pushes an element on top of the stack.
    void push(element_t);

    // Returns and removes the top element of the stack. Throws
    // `std::logic_error` if the stack is empty.
    element_t pop();

    size_t size() const;

private:
    std::vector<double> elements_;
};