#include "Cons_list.h"

#include <UnitTest++/UnitTest++.h>
#include <stdexcept>

TEST(Cons)
{
    auto x = cons(5, nullptr);
    x = cons(6, x);
    x = cons(7, x);

    CHECK_EQUAL(7, first(x));
    CHECK_EQUAL(6, first(rest(x)));
    CHECK_EQUAL(5, first(rest(rest(x))));
    CHECK(nullptr == rest(rest(rest(x))));
}

TEST(Exception)
{
    Int_list x;
    CHECK_THROW(first(x), std::logic_error);
    CHECK_THROW(rest(x), std::logic_error);
}