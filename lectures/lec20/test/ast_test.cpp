#include "ast.h"

#include <UnitTest++/UnitTest++.h>
#include <sstream>

using namespace islpp;

std::string ast2string(const Expr& expr)
{
    std::ostringstream os;
    os << *expr;
    return os.str();
}

std::string ast2string(const Decl& decl)
{
    std::ostringstream os;
    os << *decl;
    return os.str();
}

Expr vf() { return var(intern("f")); }
Expr vx() { return var(intern("x")); }
Expr vy() { return var(intern("y")); }

TEST(Integer_literal)
{
    CHECK_EQUAL("5", ast2string(int_lit(5)));
}

TEST(Boolean_literal)
{
    CHECK_EQUAL("#true", ast2string(bool_lit(true)));
    CHECK_EQUAL("#false", ast2string(bool_lit(false)));
}

TEST(String_literal)
{
    CHECK_EQUAL("\"hello\"", ast2string(string_lit("hello")));
    CHECK_EQUAL("\"hel\\\"lo\"", ast2string(string_lit("hel\"lo")));
}

TEST(Variable)
{
    CHECK_EQUAL("x", ast2string(vx()));
}

TEST(Application)
{
    CHECK_EQUAL("(f 5)", ast2string(app(vf(), {int_lit(5)})));
    CHECK_EQUAL("(f 5 x)", ast2string(app(vf(), {int_lit(5), vx()})));
}

TEST(Lambda)
{
    CHECK_EQUAL("(lambda (x) (f x))",
                ast2string(lambda({intern("x")}, app(vf(), {vx()}))));
    CHECK_EQUAL("(lambda (x y) (f x))",
                ast2string(lambda({intern("x"), intern("y")},
                                  app(vf(), {vx()}))));
}

TEST(Local)
{
    CHECK_EQUAL("(local [] 5)", ast2string(local({}, int_lit(5))));
    CHECK_EQUAL("(local [(define x 5)] x)",
                ast2string(local({define_var(intern("x"), int_lit(5))},
                                 vx())));
    CHECK_EQUAL("(local [(define x 5) (define y 6)] x)",
                ast2string(local({define_var(intern("x"), int_lit(5)),
                                  define_var(intern("y"), int_lit(6))},
                                 vx())));
}

TEST(Cond)
{
    CHECK_EQUAL("(cond [x y] [#true 5])",
                ast2string(cond({{vx(), vy()},
                                 {bool_lit(true), int_lit(5)}})));
}

TEST(Define_var)
{
    CHECK_EQUAL("(define x 7)",
                ast2string(define_var(intern("x"), int_lit(7))));
}

TEST(Define_fun)
{
    CHECK_EQUAL("(define (f x) (f x))",
                ast2string(define_fun(intern("f"),
                                      {intern("x")},
                                      app(vf(), {vx()}))));
}

TEST(Define_struct)
{
    CHECK_EQUAL("(define-struct posn [x y])",
                ast2string(define_struct(intern("posn"),
                                         {intern("x"), intern("y")})));
}
