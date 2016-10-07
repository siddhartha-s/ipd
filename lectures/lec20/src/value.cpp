#include "value.h"

namespace islpp {

/*
 * Value members
 */

int Value::as_int() const
{
    throw type_error(type(), "integer");
}

const std::string& Value::as_string() const
{
    throw type_error(type(), "string");
}

const value_ptr& Value::first() const
{
    throw type_error(type(), "cons");
}

const value_ptr& Value::rest() const
{
    throw type_error(type(), "cons");
}

const struct_id_ptr& Value::struct_id() const
{
    throw type_error(type(), "struct");
}

const value_ptr& Value::get_field(const Symbol&)
{
    throw type_error(type(), "struct");
}

value_ptr Value::operator()(const std::vector<value_ptr>&) const
{
    throw type_error(type(), "function");
}

/*
 * Integer class
 */

class Integer : public Value
{
public:
    Integer(int val) : val_(val) { }

    virtual int as_int() const override;

    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;

private:
    int val_;
};

value_ptr mk_integer(int val)
{
    return value_ptr{new Integer{val}};
}

int Integer::as_int() const
{
    return val_;
}

std::string Integer::type() const
{
    return "integer";
}

std::ostream& Integer::display(std::ostream& o) const
{
    return o << val_;
}

/*
 * String class
 */

class String : public Value
{
public:
    String(const std::string& val) : val_(val) { }

    virtual const std::string& as_string() const override;

    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;

private:
    std::string val_;
};

value_ptr mk_string(const std::string& val)
{
    return value_ptr{new String{val}};
}

const std::string& String::as_string() const
{
    return val_;
}

std::string String::type() const
{
    return "string";
}

std::ostream& String::display(std::ostream& o) const
{
    return o << val_;
}

/*
 * Cons class
 */

class Cons : public Value
{
public:
    Cons(const value_ptr& first, const value_ptr& rest)
            : first_{first}, rest_{rest} { }

    virtual const value_ptr& first() const override;

    virtual const value_ptr& rest() const override;

    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;

private:
    value_ptr first_, rest_;
};

value_ptr mk_cons(const value_ptr& first, const value_ptr& rest)
{
    return value_ptr{new Cons{first, rest}};
}

const value_ptr& Cons::first() const
{
    return first_;
}

const value_ptr& Cons::rest() const
{
    return rest_;
}

std::string Cons::type() const
{
    return "cons";
}

std::ostream& Cons::display(std::ostream& o) const
{
    o << "(cons ";
    first_->display(o);
    o << ' ';
    rest_->display(o);
    o << ')';

    return o;
}

/*
 * Empty class
 */

struct Empty : public Value
{
public:
    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;
};

value_ptr get_empty()
{
    static value_ptr instance{new Empty};
    return instance;
}

std::string Empty::type() const
{
    return "empty";
}

std::ostream& Empty::display(std::ostream& o) const
{
    return o << "'()";
}

/*
 * Struct class
 */

class Struct : public Value
{
public:
    Struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
            : id_{id}, vals_(vals) { }

    virtual std::string type() const override;

    virtual std::ostream& display(std::ostream&) const override;

private:
    struct_id_ptr          id_;
    std::vector<value_ptr> vals_;
};

value_ptr mk_struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
{
    return value_ptr{new Struct{id, vals}};
}

std::string Struct::type() const
{
    return "struct";
}

std::ostream& Struct::display(std::ostream& o) const
{
    o << "(make-";
    o << id_->name;

    for (const auto& v : vals_) {
        o << ' ';
        v->display(o);
    }

    o << ')';

    return o;
}

/*
 * Function members
 */

value_ptr Function::operator()(const std::vector<value_ptr>&) const
{
    throw std::logic_error{"Function::operator(): need to override"};
}

std::string Function::type() const
{
    return "function";
}

std::ostream& Function::display(std::ostream& o) const
{
    return o << "<#function:" << name_ << '>';
}

Function::Function(const std::string& name)
        : name_{name} { }

}


