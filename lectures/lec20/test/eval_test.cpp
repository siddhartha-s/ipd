#include "ast.h"
#include "primops.h"

#include <UnitTest++/UnitTest++.h>

using namespace islpp;

TEST(True)
{
    Expr        e = mk_bool_lit(true);
    Environment env;

    value_ptr result = e->eval(env);

    CHECK_EQUAL(get_boolean(true), result);
}

TEST(Lambda)
{
    Symbol      a = intern("a"), b = intern("b");
    Environment env;

    Expr l5 = mk_int_lit(5);
    Expr l6 = mk_int_lit(6);

    Expr f = mk_lambda({a, b}, mk_var(a));
    Expr g = mk_lambda({a, b}, mk_var(b));

    CHECK_EQUAL(5, mk_app(f, {l5, l6})->eval(env)->as_int());
    CHECK_EQUAL(6, mk_app(g, {l5, l6})->eval(env)->as_int());
}

TEST(Factorial)
{
    Symbol n    = intern("n"),
           fact = intern("fact");

    Expr body = mk_cond(
            {{mk_app(mk_var(intern("zero?")), {mk_var(n)}),
                     mk_int_lit(1)},
             {mk_bool_lit(true),
                     mk_app(mk_var(intern("*")),
                            {mk_var(n),
                             mk_app(mk_var(fact),
                                    {mk_app(mk_var(intern("-")),
                                            {mk_var(n), mk_int_lit(1)})})})}});

    Expr e = mk_local({mk_define_fun(fact, {n}, body)},
                      mk_app(mk_var(fact), {mk_int_lit(5)}));

    CHECK_EQUAL(120, e->eval(primop::environment)->as_int());
}
