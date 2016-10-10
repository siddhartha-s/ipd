#pragma once

#include "lexer.h"
#include "reader.h"
#include "ast.h"

#include <stdexcept>

namespace islpp {

Expr parse_expr(const value_ptr&);
Expr parse_expr(Lexer&);
Expr parse_expr(std::istream&);

Decl parse_decl(const value_ptr&);
Decl parse_decl(Lexer&);
Decl parse_decl(std::istream&);

}
