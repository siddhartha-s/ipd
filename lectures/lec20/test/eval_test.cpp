#include "ast.h"
#include "primops.h"
#include "parser.h"

#include <UnitTest++/UnitTest++.h>
#include <sstream>

using namespace islpp;

value_ptr ev_string(const std::string& str)
{
    std::istringstream input(str);
    return parse_expr(input)->eval(primop::environment);
}

TEST(True)
{
    CHECK_EQUAL(get_boolean(true), ev_string("#true"));
}

TEST(Lambda)
{
    Symbol      a = intern("a"), b = intern("b");
    Environment env;

    Expr l5 = int_lit(5);
    Expr l6 = int_lit(6);

    Expr f = lambda({a, b}, var(a));
    Expr g = lambda({a, b}, var(b));

    CHECK_EQUAL(5, app(f, {l5, l6})->eval(env)->as_int());
    CHECK_EQUAL(6, app(g, {l5, l6})->eval(env)->as_int());
}

TEST(Factorial)
{
    Symbol n    = intern("n"),
           fact = intern("fact");

    Expr body = cond(
            {{app(var(intern("zero?")), {var(n)}),
                     int_lit(1)},
             {bool_lit(true),
                     app(var(intern("*")),
                         {var(n),
                          app(var(fact),
                              {app(var(intern("-")),
                                   {var(n), int_lit(1)})})})}});

    Expr e = local({define_fun(fact, {n}, body)},
                   app(var(fact), {int_lit(5)}));

    CHECK_EQUAL(120, e->eval(primop::environment)->as_int());
}

TEST(Posn)
{
    Symbol posn = intern("posn");

    Decl decl1 = define_struct(posn, {intern("x"), intern("y")});
    Decl decl2 = define_var(intern("p"),
                            app(var(intern("make-posn")),
                                {int_lit(3), int_lit(4)}));

    CHECK_EQUAL(get_boolean(true),
                local({decl1, decl2},
                      app(var(intern("posn?")), {var(intern("p"))}))
                        ->eval(primop::environment));
    CHECK_EQUAL(get_boolean(false),
                local({decl1, decl2},
                      app(var(intern("cons?")), {var(intern("p"))}))
                        ->eval(primop::environment));
    CHECK_EQUAL(3,
                local({decl1, decl2},
                      app(var(intern("posn-x")), {var(intern("p"))}))
                        ->eval(primop::environment)->as_int());
    CHECK_EQUAL(4,
                local({decl1, decl2},
                      app(var(intern("posn-y")), {var(intern("p"))}))
                        ->eval(primop::environment)->as_int());
}

TEST(Factorial_again)
{
    CHECK_EQUAL(120, ev_string(
            "(local [(define (fact n)"
            "          (cond"
            "           [(zero? n) 1]"
            "           [else (* n (fact (- n 1)))]))]"
            "  (fact 5))")
            ->as_int());
}

TEST(EvenOdd)
{
    CHECK_EQUAL(get_boolean(true), ev_string(
            "(local [(define (even? n)"
            "          (cond [(zero? n) #t]"
            "                [else (odd? (- n 1))]))"
            "        (define (odd? n)"
            "          (cond [(zero? n) #f]"
            "                [else (even? (- n 1))]))]"
            "  (even? 6))"));
}

TEST(SumList)
{
    CHECK_EQUAL(10, ev_string(
            "(local [(define (sum lst)"
            "          (cond [(empty? lst) 0]"
            "                [else (+ (first lst) (sum (rest lst)))]))]"
            "  (sum (cons 1 (cons 2 (cons 3 (cons 4 empty))))))")
                ->as_int());
}
