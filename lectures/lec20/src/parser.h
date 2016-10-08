#pragma once

#include "lexer.h"
#include "ast.h"

#include <stdexcept>

namespace islpp {

Expr parse_expr(std::istream&);
Expr parse_expr(Lexer&);

Decl parse_decl(std::istream&);
Decl parse_decl(Lexer&);

}
