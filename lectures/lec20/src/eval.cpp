#include "ast.h"
#include "value.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

namespace islpp {

class Closure : public Function
{
public:
    Closure(const Symbol& name,
            const std::vector<Symbol>& formals,
            const Expr& body,
            const Environment& env)
            : Function(name, static_cast<ssize_t>(formals.size())),
              formals_(formals), body_(body),
              env_(env) { }

    virtual value_ptr apply(const std::vector<value_ptr>&) const override;

private:
    std::vector<Symbol> formals_;
    Expr                body_;
    Environment         env_;
};

value_ptr mk_closure(const Symbol& name,
                     const std::vector<Symbol>& formals,
                     const Expr& body,
                     const Environment& env)
{
    return std::make_shared<Closure>(name, formals, body, env);
}

value_ptr Closure::apply(const std::vector<value_ptr>& actuals) const
{
    // Checked by operator():
    assert(actuals.size() == formals_.size());

    Environment env    = env_;
    auto        formal = formals_.begin();
    auto        actual = actuals.begin();

    for (; formal != formals_.end(); ++formal, ++actual) {
        env = env.extend(*formal, *actual);
    }

    return body_->eval(env);
}

class Constructor : public Function
{
public:
    Constructor(const Symbol& name, const struct_id_ptr& id)
            : Function(name, id->fields.size()), id_(id) { }

    virtual value_ptr apply(const std::vector<value_ptr>&) const override;

private:
    struct_id_ptr id_;
};


value_ptr Constructor::apply(const std::vector<value_ptr>& actuals) const
{
    return mk_struct(id_, actuals);
}

class Predicate : public Function
{
public:
    Predicate(const Symbol& name, const struct_id_ptr& id)
            : Function(name, 1), id_(id) { }

    virtual value_ptr apply(const std::vector<value_ptr>&) const override;

private:
    struct_id_ptr id_;
};

value_ptr Predicate::apply(const std::vector<value_ptr>& actuals) const
{
    return get_boolean(actuals[0]->type() == value_type::Struct
                       && actuals[0]->struct_id() == id_);
}

class Selector : public Function
{
public:
    Selector(const Symbol& name, const struct_id_ptr& id, size_t field)
            : Function(name, 1), id_(id), field_(field) { }

    virtual value_ptr apply(const std::vector<value_ptr>&) const override;

private:
    struct_id_ptr id_;
    size_t field_;
};

value_ptr Selector::apply(const std::vector<value_ptr>& actuals) const
{
    if (actuals[0]->type() == value_type::Struct
        && actuals[0]->struct_id() == id_) {
        return actuals[0]->get_fields()[field_];
    } else {
        throw type_error(actuals[0]->type_name(),
                         "struct " + id_->name.name());
    }
}

value_ptr Variable::eval(const Environment& env) const
{
    value_ptr result = env.lookup(name_);

    if (result->type() == value_type::Undefined)
        throw std::runtime_error("used not-yet-defined variable");

    return result;
}

value_ptr Application::eval(const Environment& env) const
{
    value_ptr fun = fun_->eval(env);

    std::vector<value_ptr> actuals;

    for (const auto& actual : actuals_)
        actuals.push_back(actual->eval(env));

    return (*fun)(actuals);
}

value_ptr Lambda::eval(const Environment& env) const
{
    return mk_closure(intern("lambda"), formals_, body_, env);
}

value_ptr Cond::eval(const Environment& env) const
{
    for (const auto& alt : alts_) {
        if (alt.first->eval(env)->as_bool())
            return alt.second->eval(env);
    }

    throw std::runtime_error{"cond: all questions were false"};
}

value_ptr Local::eval(const Environment& env0) const
{
    Environment env = env0;

    for (const auto& decl : decls_)
        env = decl->extend(env);

    for (const auto& decl : decls_)
        decl->eval(env);

    return body_->eval(env);
}

value_ptr Integer_literal::eval(const Environment&) const
{
    return mk_integer(val_);
}

value_ptr String_literal::eval(const Environment&) const
{
    return mk_string(val_);
}

value_ptr Boolean_literal::eval(const Environment&) const
{
    return get_boolean(val_);
}

Environment Define_var::extend(const Environment& env) const
{
    return env.extend(name_, get_undefined());
}

void Define_var::eval(Environment& env) const
{
    env.update(name_, rhs_->eval(env));
}

Environment Define_fun::extend(const Environment& env) const
{
    return env.extend(name_, get_undefined());
}

void Define_fun::eval(Environment& env) const
{
    env.update(name_, mk_closure(name_, formals_, body_, env));
}

Environment Define_struct::extend(const Environment& env0) const
{
    Environment env = env0;

    env = env.extend("make-" + name_.name(), get_undefined());
    env = env.extend(name_.name() + "?", get_undefined());
    for (const auto& field : fields_)
        env = env.extend(name_.name() + "-" + field.name(), get_undefined());

    return env;
}

void Define_struct::eval(Environment& env) const
{
    struct_id_ptr id = std::make_shared<Struct_id>(name_, fields_);

    Symbol constructor = intern("make-" + name_.name());
    env.update(constructor, std::make_shared<Constructor>(constructor, id));

    Symbol predicate = intern(name_.name() + "?");
    env.update(predicate, std::make_shared<Predicate>(predicate, id));

    for (size_t i = 0; i < fields_.size(); ++i) {
        Symbol selector = intern(name_.name() + "-" + fields_[i].name());
        env.update(selector, std::make_shared<Selector>(selector, id, i));
    }
}

Environment Expr_decl::extend(const Environment& env0) const
{
    return env0;
}

void Expr_decl::eval(Environment& env) const
{
    std::cout << expr_->eval(env) << '\n';
}

}
