#pragma once

#include "env.h"
#include "symbol.h"
#include "value.h"

#include <memory>
#include <utility>

namespace islpp {

using Environment = env_ptr<value_ptr>;

class Expr_node;

class Decl_node;

using Expr = std::shared_ptr<Expr_node>;
using Decl = std::shared_ptr<Decl_node>;

class Expr_node
{
public:
    virtual value_ptr eval(const Environment&) const = 0;
};

class Decl_node
{
public:
    virtual Environment extend(const Environment&) const = 0;
    virtual void eval(Environment&) const = 0;
};

Expr mk_variable(const Symbol& name);
Expr mk_application(const Expr& fun, const std::vector<Expr>& actuals);
Expr mk_lambda(const std::vector<Symbol>& formals, const Expr& body);
Expr mk_local(const std::vector<Decl>& decls, const Expr& body);
Expr mk_cond(const std::vector<std::pair<Expr, Expr>>& alts);
Expr mk_integer_literal(int val);
Expr mk_string_literal(const std::string& val);
Expr mk_boolean_literal(bool val);

Decl mk_define_var(const Symbol& name, const Expr& rhs);
Decl mk_define_fun(const Symbol& name, const std::vector<Symbol>& formals,
                   const Expr& body);
Decl mk_define_struct(const Symbol& name, const std::vector<Symbol>& fields);

/*
 * Expressions
 */

class Variable : public Expr_node
{
public:
    Variable(const Symbol& name) : name_(name) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    Symbol name_;
};

class Application : public Expr_node
{
public:
    Application(const Expr& fun, const std::vector<Expr>& actuals)
            : fun_(fun), actuals_(actuals) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    Expr fun_;
    std::vector<Expr> actuals_;
};

class Lambda : public Expr_node
{
public:
    Lambda(const std::vector<Symbol>& formals, const Expr& body)
            : formals_(formals), body_(body) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    std::vector<Symbol> formals_;
    Expr                body_;
};

class Local : public Expr_node
{
public:
    Local(const std::vector<Decl>& decls, const Expr& body)
            : decls_{decls}, body_{body} { }

    virtual value_ptr eval(const Environment&) const override;

private:
    std::vector<Decl> decls_;
    Expr body_;
};

class Cond : public Expr_node
{
public:
    Cond(const std::vector<std::pair<Expr, Expr>>& alts)
            : alts_(alts) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    std::vector<std::pair<Expr, Expr>> alts_;
};

class Integer_literal : public Expr_node
{
public:
    Integer_literal(int val)
            : val_(val) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    int val_;
};

class String_literal : public Expr_node
{
public:
    String_literal(const std::string& val)
            : val_(val) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    std::string val_;
};

class Boolean_literal : public Expr_node
{
public:
    Boolean_literal(bool val)
            : val_(val) { }

    virtual value_ptr eval(const Environment&) const override;

private:
    bool val_;
};

/*
 * Declarations
 */

class Define_var : public Decl_node
{
public:
    Define_var(const Symbol& name, const Expr& rhs)
            : name_(name), rhs_(rhs) { }

    virtual Environment extend(const Environment&) const override;
    virtual void eval(Environment&) const override;

private:
    Symbol name_;
    Expr rhs_;
};

class Define_fun : public Decl_node
{
public:
    Define_fun(const Symbol& name, const std::vector<Symbol>& formals,
           const Expr& body)
            : name_(name), formals_(formals), body_(body) { }

    virtual Environment extend(const Environment&) const override;
    virtual void eval(Environment&) const override;

private:
    Symbol name_;
    std::vector<Symbol> formals_;
    Expr body_;
};

class Define_struct : public Decl_node
{
public:
    Define_struct(const Symbol& name, const std::vector<Symbol>& fields)
            : name_(name), fields_(fields) { }

    virtual Environment extend(const Environment&) const override;
    virtual void eval(Environment&) const override;

private:
    Symbol name_;
    std::vector<Symbol> fields_;
};

}
