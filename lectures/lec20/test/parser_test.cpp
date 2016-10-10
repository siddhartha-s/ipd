#include "parser.h"

#include <UnitTest++/UnitTest++.h>
#include <sstream>

using namespace islpp;

Expr parse_string(const std::string& str)
{
    std::istringstream input(str);
    return parse_expr(input);
}

TEST(Things_that_parse)
{
    parse_string("5");
    parse_string("(+ a 5)");
    parse_string("(local [(define a 5)] (+ a 6))");
}

TEST(Things_that_do_not_parse)
{
    CHECK_THROW(parse_string("(+ a 5"), syntax_error);
    CHECK_THROW(parse_string("(local (define a 5) (+ a 6))"),
                syntax_error);

}