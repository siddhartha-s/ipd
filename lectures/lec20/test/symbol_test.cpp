#include "symbol.h"
#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(Intern)
{
    auto& tab = Intern_table::INSTANCE();
    Symbol a1 = tab.intern("a"),
           a2 = tab.intern("a"),
           b1 = tab.intern("b"),
           b2 = tab.intern("b");

    CHECK_EQUAL(a1, a1);
    CHECK_EQUAL(a1, a2);
    CHECK_EQUAL(b1, b2);

    CHECK(a1 != b1);
}

TEST(Uninterned)
{
    auto& tab = Intern_table::INSTANCE();
    Symbol a1 = tab.intern("a"),
           a2 = Symbol::uninterned("a"),
           a3 = Symbol::uninterned("a");

    CHECK(a1 != a2);
    CHECK(a1 != a3);
    CHECK(a2 != a3);
}