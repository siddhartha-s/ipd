#include "value.h"

namespace islpp {

const char* to_string(value_type vt)
{
    switch (vt) {
        case value_type::Boolean:
            return "boolean";
        case value_type::Integer:
            return "integer";
        case value_type::String:
            return "string";
        case value_type::Cons:
            return "cons";
        case value_type::Empty:
            return "empty";
        case value_type::Struct:
            return "struct";
        case value_type::Function:
            return "function";
        case value_type::Void:
            return "void";
    }
}

std::ostream& operator<<(std::ostream& o, value_type vt)
{
    return o << to_string(vt);
}

/*
 * Value members
 */

bool Value::as_bool() const
{
    throw type_error(to_string(type()), "boolean");
}

int Value::as_int() const
{
    throw type_error(to_string(type()), "integer");
}

const std::string& Value::as_string() const
{
    throw type_error(to_string(type()), "string");
}

const value_ptr& Value::first() const
{
    throw type_error(to_string(type()), "cons");
}

const value_ptr& Value::rest() const
{
    throw type_error(to_string(type()), "cons");
}

const struct_id_ptr& Value::struct_id() const
{
    throw type_error(to_string(type()), "struct");
}

const value_ptr& Value::get_field(const Symbol&)
{
    throw type_error(to_string(type()), "struct");
}

value_ptr Value::operator()(const std::vector<value_ptr>&) const
{
    throw type_error(to_string(type()), "function");
}

/*
 * Boolean class
 */

class Boolean : public Value
{
public:
    Boolean(bool val) : val_(val) { }

    virtual bool as_bool() const override;

    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;

private:
    bool val_;
};

value_ptr get_boolean(bool val)
{
    static value_ptr true_instance{new Boolean{true}};
    static value_ptr false_instance{new Boolean{false}};

    return val ? true_instance : false_instance;
}

bool Boolean::as_bool() const
{
    return val_;
}

value_type Boolean::type() const
{
    return value_type::Boolean;
}

std::ostream& Boolean::display(std::ostream& o) const
{
    return o << (val_ ? "#true" : "#false");
}

/*
 * Integer class
 */

class Integer : public Value
{
public:
    Integer(int val) : val_(val) { }

    virtual int as_int() const override;

    virtual value_type type() const override;
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

value_type Integer::type() const
{
    return value_type::Integer;
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

    virtual value_type type() const override;
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

value_type String::type() const
{
    return value_type::String;
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

    virtual value_type type() const override;
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

value_type Cons::type() const
{
    return value_type::Cons;
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
    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
};

value_ptr get_empty()
{
    static value_ptr instance{new Empty};
    return instance;
}

value_type Empty::type() const
{
    return value_type::Empty;
}

std::ostream& Empty::display(std::ostream& o) const
{
    return o << "'()";
}

/*
 * Void class
 */

struct Void : public Value
{
public:
    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
};

value_ptr get_void()
{
    static value_ptr instance{new Void};
    return instance;
}

value_type Void::type() const
{
    return value_type::Void;
}

std::ostream& Void::display(std::ostream& o) const
{
    return o << "#<void>";
}

/*
 * Struct class
 */

class Struct : public Value
{
public:
    Struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
            : id_{id}, vals_(vals) { }

    virtual value_type type() const override;

    virtual std::ostream& display(std::ostream&) const override;

private:
    struct_id_ptr          id_;
    std::vector<value_ptr> vals_;
};

value_ptr mk_struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
{
    return value_ptr{new Struct{id, vals}};
}

value_type Struct::type() const
{
    return value_type::Struct;
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

value_ptr Function::operator()(const std::vector<value_ptr>& args) const
{
    if (arity_ < 0 ? args.size() < -1 - arity_ : args.size() != arity_)
        throw arity_error(args.size(), arity_);

    return apply(args);
}

value_type Function::type() const
{
    return value_type::Function;
}

std::ostream& Function::display(std::ostream& o) const
{
    return o << "<#function:" << name_ << '>';
}

Function::Function(const std::string& name, ssize_t arity)
        : arity_{arity}, name_{name} { }

}


