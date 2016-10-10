#include "parser.h"

#include <UnitTest++/UnitTest++.h>
#include <sstream>

using namespace islpp;

std::string rt_expr(const std::string& str)
{
    std::istringstream input(str);
    std::ostringstream output;
    output << *parse_expr(input);
    return output.str();
}

#define CHECK_EXPR(expr) CHECK_EQUAL(expr, rt_expr(expr))

TEST(Integer_literal)
{
    CHECK_EXPR("5");
    CHECK_EXPR("-5");
    CHECK_EQUAL("5", rt_expr("+5"));
}

TEST(String_literal)
{
    CHECK_EXPR("\"hello\"");
}

TEST(Boolean_literal)
{
    CHECK_EXPR("#true");
    CHECK_EXPR("#false");
}

TEST(Variable)
{
    CHECK_EXPR("a");
}

TEST(Application)
{
    CHECK_EXPR("(f x)");
    CHECK_EQUAL("(f x 5)", rt_expr("(f x   5)"));
}

TEST(Lambda)
{
    CHECK_EXPR("(lambda (x y) (+ x y))");
}

TEST(Local)
{
    CHECK_EXPR("(local [] 5)");
    CHECK_EXPR("(local [(define x 5)] x)");
}

TEST(Cond)
{
    CHECK_EXPR("(cond [(zero? n) 1] [#true (* n (fact (- n 1)))])");
    CHECK_EQUAL("(cond [#true a])", rt_expr("(cond [else a])"));
}

TEST(Define_var)
{
    CHECK_EXPR("(local [(define a 5) (define b 8)] (+ a b))");
}

TEST(Define_fun)
{
    CHECK_EXPR("(local [(define (f x) (+ 1 x))] (f 3))");
}

TEST(Define_struct)
{
    CHECK_EXPR("(local [(define-struct posn [x y])] (make-posn 3 4))");
}

//TEST(Things_that_do_not_parse)
//{
//    CHECK_THROW(parse_string("(+ a 5"), syntax_error);
//    CHECK_THROW(parse_string("(local (define a 5) (+ a 6))"),
//                syntax_error);
//
//}