#include "primops.h"
#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(Plus)
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = (*primop::plus)({a, b});

    CHECK_EQUAL(5, c->as_int());
}

TEST(PlusMore)
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = mk_integer(4),
              d = (*primop::plus)({a, b, c});

    CHECK_EQUAL(9, d->as_int());
}

TEST(Cons)
{
    value_ptr a = mk_integer(5),
              b = get_empty(),
              c = (*primop::cons)({a, b});

    CHECK_EQUAL(5, c->first()->as_int());
    CHECK_EQUAL(value_type::Empty, c->rest()->type());
}

TEST(Cons3)
{
    value_ptr a = mk_integer(5);
    CHECK_THROW((*primop::cons)({a, a, a}), arity_error);
}

TEST(NumEq)
{
    value_ptr a = mk_integer(5),
              b = mk_integer(5),
              c = mk_integer(6),
              d = get_boolean(true);

    CHECK_EQUAL(get_boolean(true), (*primop::num_eq)({a, b}));
    CHECK_EQUAL(get_boolean(false), (*primop::num_eq)({a, c}));

    CHECK_THROW((*primop::num_eq)({a, d}), type_error);
    CHECK_THROW((*primop::num_eq)({d, a}), type_error);
    CHECK_THROW((*primop::num_eq)({d, d}), type_error);
}

TEST(EqualHuh)
{
    value_ptr a = mk_integer(4),
              b = mk_integer(5),
              c = mk_cons(b, get_empty()),
              d = mk_cons(a, c);

    CHECK_EQUAL(get_boolean(true), (*primop::equal_huh)({a, a}));
    CHECK_EQUAL(get_boolean(true), (*primop::equal_huh)({b, b}));
    CHECK_EQUAL(get_boolean(true), (*primop::equal_huh)({c, c}));
    CHECK_EQUAL(get_boolean(true), (*primop::equal_huh)({d, d}));

    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({a, b}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({a, c}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({a, d}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({b, a}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({b, c}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({b, d}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({c, a}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({c, b}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({c, d}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({d, a}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({d, b}));
    CHECK_EQUAL(get_boolean(false), (*primop::equal_huh)({d, c}));
}
