#include "Queue.h"

void Queue::enqueue(const std::string& s)
{
    back.push_back(s);
}

std::string Queue::dequeue()
{
    if (front.empty()) {
        if (back.empty())
            throw std::logic_error("Queue::dequeue(): empty queue");

        while (!back.empty()) {
            front.push_back(back.back());
            back.pop_back();
        }
    }

    std::string result = front.back();
    front.pop_back();
    return result;
}

size_t Queue::size() const
{
    return front.size() + back.size();
}








