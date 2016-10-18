#pragma once

#include "lexer.h"
#include "reader.h"
#include "ast.h"

#include <stdexcept>

namespace islpp {

Expr parse_expr(const value_ptr&);
Expr parse_expr(Lexer&);
Expr parse_expr(std::istream&);

Decl parse_decl(const value_ptr&, bool allow_expr = false);
Decl parse_decl(Lexer&, bool allow_expr = false);
Decl parse_decl(std::istream&, bool allow_expr = false);

Prog parse_prog(Lexer&);
Prog parse_prog(std::istream&);

}
