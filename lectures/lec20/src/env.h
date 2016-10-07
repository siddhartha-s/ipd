#pragma once

// Environments are reference counted linked lists.

#include <memory>
#include <string>

namespace islpp {

template<typename V>
class env_ptr
{
public:
    const V& lookup(std::string);

private:
    struct node
    {
        std::string key;
        V           value;
        node_ptr    next;
    };
    using node_ptr = std::shared_ptr<node>;

    node_ptr head_;
};

}
