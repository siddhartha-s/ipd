#include "node.hpp"

namespace graph
{

node::node() noexcept : node(0) { }

node::node(size_t value) noexcept : value_(value) { }

size_t node::to_index() const noexcept
{
    return value_ - FIRST.value_;
}

const node node::INVALID{};

const node node::FIRST{1};

node& node::operator++() noexcept
{
    ++this->value_;
    return *this;
}

node node::operator++(int) noexcept
{
    node result{*this};
    ++this->value_;
    return result;
}

std::ostream&
operator<<(std::ostream& out, node n)
{
    if (n == node::INVALID)
        return out << "node::INVALID";
    else
        return out << "node{" << n.to_index() << "}";
}

} // namespace graph
