#include "primops.h"
#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(Plus)
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = (*primops::plus)({a, b});

    CHECK_EQUAL(5, c->as_int());
}

TEST(PlusMore)
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = mk_integer(4),
              d = (*primops::plus)({a, b, c});

    CHECK_EQUAL(9, d->as_int());
}

TEST(Cons)
{
    value_ptr a = mk_integer(5),
              b = get_empty(),
              c = (*primops::cons)({a, b});

    CHECK_EQUAL(5, c->first()->as_int());
    CHECK_EQUAL(value_type::Empty, c->rest()->type());
}

TEST(Cons3)
{
    value_ptr a = mk_integer(5);
    CHECK_THROW((*primops::cons)({a, a, a}), arity_error);
}


