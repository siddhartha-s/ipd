#pragma once

#include "value.h"
#include "lexer.h"

namespace islpp {

value_ptr read(Lexer&);

struct syntax_error : std::runtime_error
{
    syntax_error(const std::string& s)
            : std::runtime_error("Syntax error: " + s) {}

    syntax_error(const std::string& got, const std::string& exp)
            : std::runtime_error("got " + got + " where " + exp +
                                 " expected") {}
};

}