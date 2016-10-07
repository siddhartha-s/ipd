#include "value.h"

#include <UnitTest++/UnitTest++.h>
#include <sstream>

using namespace islpp;

TEST(Boolean)
{
    value_ptr t = mk_boolean(true);
    value_ptr f = mk_boolean(false);

    CHECK_EQUAL(true, t->as_bool());
    CHECK_EQUAL(false, f->as_bool());
    CHECK_THROW(t->as_int(), type_error);

    std::ostringstream os;
    os << t << ' ' << f;
    CHECK_EQUAL("#true #false", os.str());
}

TEST(Integer)
{
    value_ptr i = mk_integer(5);

    CHECK_EQUAL(5, i->as_int());
    CHECK_THROW(i->as_bool(), type_error);

    std::ostringstream os;
    os << i;
    CHECK_EQUAL("5", os.str());
}

