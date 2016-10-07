#include "env.h"
#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(LookupEmpty)
{
    env_ptr<int> empty;
    Symbol a = intern("a");

    CHECK_THROW(empty.lookup(a), binding_not_found);
}

TEST(LookupBound)
{
    Symbol a = intern("a"),
           b = intern("b"),
           c = intern("c"),
           d = intern("d");
    env_ptr<int> env = env_ptr<int>{}.bind(a, 2).bind(b, 3).bind(c, 4);

    CHECK_EQUAL(2, env.lookup(a));
    CHECK_EQUAL(3, env.lookup(b));
    CHECK_EQUAL(4, env.lookup(c));
    CHECK_THROW(env.lookup(d), binding_not_found);
}

TEST(LookupShadow)
{
    Symbol a = intern("a"),
           b = intern("b");
    env_ptr<int> env1 = env_ptr<int>{}.bind(a, 2).bind(b, 3);
    env_ptr<int> env2 = env1.bind(a, 5);

    CHECK_EQUAL(2, env1.lookup(a));
    CHECK_EQUAL(3, env1.lookup(b));
    CHECK_EQUAL(5, env2.lookup(a));
    CHECK_EQUAL(3, env2.lookup(b));
}

TEST(Update)
{
    Symbol a = intern("a"),
           b = intern("b");
    env_ptr<int> env1 = env_ptr<int>{}.bind(a, 2).bind(b, 3);
    env_ptr<int> env2 = env1.bind(a, 5);

    env2.update(a, 6);
    CHECK_EQUAL(2, env1.lookup(a));
    CHECK_EQUAL(3, env1.lookup(b));
    CHECK_EQUAL(6, env2.lookup(a));
    CHECK_EQUAL(3, env2.lookup(b));
}
