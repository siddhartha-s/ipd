#pragma once

#include <cstddef>
#include <string>
#include <vector>

// A queue (FIFO) of strings.
class Queue
{
public:
    // Constructs a new, empty queue.
    Queue();

    // Enqueues a string to the end of the queue.
    void enqueue(const std::string&);

    // Dequeues and returns the front string, or throws `std::logic_error`
    // if empty.
    std::string dequeue();

    // Returns the number of elements in the queue.
    size_t size() const;

private:
    std::vector<std::string> front;
    std::vector<std::string> back;
};
