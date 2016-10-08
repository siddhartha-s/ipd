#include "ast.h"
#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(True) {
    Expr e = mk_boolean_literal(true);
    Environment env;

    value_ptr result = e->eval(env);

    CHECK_EQUAL(get_boolean(true), result);
}

TEST(Lambda) {
    Symbol a = intern("a"), b = intern("b");
    Environment env;

    Expr l5 = mk_integer_literal(5);
    Expr l6 = mk_integer_literal(6);

    Expr f = mk_lambda({a, b}, mk_variable(a));
    Expr g = mk_lambda({a, b}, mk_variable(b));

    CHECK_EQUAL(5, mk_application(f, {l5, l6})->eval(env)->as_int());
    CHECK_EQUAL(6, mk_application(g, {l5, l6})->eval(env)->as_int());
}
