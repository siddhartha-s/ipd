#pragma once

// Environments are reference counted linked lists.

#include "symbol.h"

#include <memory>
#include <string>

namespace islpp {

struct binding_not_found : std::runtime_error
{
    binding_not_found(const Symbol& sym)
            : runtime_error("Not found: " + sym.name()) { }
};

template<typename V>
class env_ptr
{
public:
    env_ptr();

    env_ptr bind(const Symbol&, const V&) const;

    void update(const Symbol&, const V&);

    const V& lookup(const Symbol&) const;

private:
    struct node;
    using node_ptr = std::shared_ptr<node>;

    env_ptr(const Symbol&, const V&, const node_ptr&);

    node_ptr head_;
};

template<typename V>
struct env_ptr<V>::node
{
    node(const Symbol& k, const V& v, const node_ptr& n)
            : key(k), value(v), next(n) { }

    Symbol   key;
    V        value;
    node_ptr next;
};

template<typename V>
env_ptr<V>::env_ptr()
        : head_{nullptr} { }

template<typename V>
env_ptr<V>::env_ptr(const Symbol& key, const V& value, const node_ptr& next)
        : head_{std::make_shared<node>(key, value, next)} { }

template<typename V>
const V& env_ptr<V>::lookup(const Symbol& key) const
{
    for (node* curr = &*head_; curr != nullptr; curr = &*curr->next) {
        if (curr->key == key) return curr->value;
    }

    throw binding_not_found{key};
}

template<typename V>
env_ptr<V> env_ptr<V>::bind(const Symbol& key, const V& value) const
{
    return env_ptr<V>{key, value, head_};
}

template<typename V>
void env_ptr<V>::update(const Symbol& key, const V& value)
{
    for (node* curr = &*head_; curr != nullptr; curr = &*curr->next) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
    }

    throw binding_not_found{key};
}

}
