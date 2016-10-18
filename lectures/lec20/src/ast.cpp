#include "ast.h"

namespace islpp {

std::ostream& operator<<(std::ostream& o, const Expr_node& e)
{
    return e.display(o);
}

std::ostream& operator<<(std::ostream& o, const Decl_node& d)
{
    return d.display(o);
}

Expr var(const Symbol& name)
{
    return std::make_shared<Variable>(name);
}

Expr app(const Expr& fun, const std::vector<Expr>& actuals)
{
    return std::make_shared<Application>(fun, actuals);
}

Expr lambda(const std::vector<Symbol>& formals, const Expr& body)
{
    return std::make_shared<Lambda>(formals, body);
}

Expr local(const std::vector<Decl>& decls, const Expr& body)
{
    return std::make_shared<Local>(decls, body);
}

Expr cond(const std::vector<std::pair<Expr, Expr>>& alts)
{
    return std::make_shared<Cond>(alts);
}

Expr int_lit(int val)
{
    return std::make_shared<Integer_literal>(val);
}

Expr string_lit(const std::string& val)
{
    return std::make_shared<String_literal>(val);
}

Expr bool_lit(bool val)
{
    return std::make_shared<Boolean_literal>(val);
}

Decl define_var(const Symbol& name, const Expr& rhs)
{
    return std::make_shared<Define_var>(name, rhs);
}

Decl define_fun(const Symbol& name, const std::vector<Symbol>& formals,
                const Expr& body)
{
    return std::make_shared<Define_fun>(name, formals, body);
}

Decl define_struct(const Symbol& name, const std::vector<Symbol>& fields)
{
    return std::make_shared<Define_struct>(name, fields);
}

Decl expr_decl(const Expr& expr)
{
    return std::make_shared<Expr_decl>(expr);
}

std::ostream& Variable::display(std::ostream& o) const
{
    return o << name_;
}

std::ostream& Application::display(std::ostream& o) const
{
    o << '(' << *fun_;
    for (const auto& actual : actuals_) o << ' ' << *actual;
    return o << ')';
}

std::ostream& Lambda::display(std::ostream& o) const
{
    o << "(lambda (";
    for (size_t i = 0; i < formals_.size(); ++i) {
        if (i != 0) o << ' ';
        o << formals_[i];
    }
    return o << ") " << *body_ << ')';
}

std::ostream& Local::display(std::ostream& o) const
{
    o << "(local [";
    for (size_t i = 0; i < decls_.size(); ++i) {
        if (i != 0) o << ' ';
        o << *decls_[i];
    }
    return o << "] " << *body_ << ')';
}

std::ostream& Cond::display(std::ostream& o) const
{
    o << "(cond";
    for (const auto& alt : alts_) {
        o << " [" << *alt.first << ' ' << *alt.second << ']';
    }
    return o << ')';
}

std::ostream& Integer_literal::display(std::ostream& o) const
{
    return o << val_;
}

std::ostream& String_literal::display(std::ostream& o) const
{
    o << '"';

    for (char c : val_) {
        switch (c) {
            case '"': case '\\':
                o << '\\' << c;
                break;

            case '\n':
                o << "\\n";
                break;

            case '\t':
                o << "\\t";
                break;

            case '\r':
                o << "\\r";
                break;

            default:
                o << c;
        }
    }

    return o << '"';
}

std::ostream& Boolean_literal::display(std::ostream& o) const
{
    return o << (val_ ? "#true" : "#false");
}

std::ostream& Define_var::display(std::ostream& o) const
{
    return o << "(define " << name_ << ' ' << *rhs_ << ')';
}

std::ostream& Define_fun::display(std::ostream& o) const
{
    o << "(define (";
    o << name_;
    for (const auto& formal : formals_)
        o << ' ' << formal;
    return o << ") " << *body_ << ')';
}

std::ostream& Define_struct::display(std::ostream& o) const
{
    o << "(define-struct " << name_ << " [";
    for (size_t i = 0; i < fields_.size(); ++i) {
        if (i != 0) o << ' ';
        o << fields_[i];
    }
    return o << "])";
}

std::ostream& Expr_decl::display(std::ostream& o) const
{
    return o << expr_;
}

}

