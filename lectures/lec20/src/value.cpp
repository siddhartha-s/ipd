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
        case value_type::Symbol:
            return "symbol";
        case value_type::Cons:
            return "cons";
        case value_type::Empty:
            return "empty";
        case value_type::Struct:
            return "struct";
        case value_type::Function:
            return "function";
        case value_type::Undefined:
            return "undefined";
    }
}

std::ostream& operator<<(std::ostream& o, value_type vt)
{
    return o << to_string(vt);
}

/*
 * Value members
 */

std::string Value::type_name() const
{
    return to_string(type());
}

bool Value::as_bool() const
{
    throw type_error(type_name(), "boolean");
}

int Value::as_int() const
{
    throw type_error(type_name(), "integer");
}

const std::string& Value::as_string() const
{
    throw type_error(type_name(), "string");
}

const Symbol& Value::as_symbol() const
{
    throw type_error(type_name(), "symbol");
}

const value_ptr& Value::first() const
{
    throw type_error(type_name(), "cons");
}

const value_ptr& Value::rest() const
{
    throw type_error(type_name(), "cons");
}

const struct_id_ptr& Value::struct_id() const
{
    throw type_error(type_name(), "struct");
}

const std::vector<value_ptr>& Value::get_fields() const
{
    throw type_error(type_name(), "struct");
}

value_ptr Value::operator()(const std::vector<value_ptr>&) const
{
    throw type_error(type_name(), "function");
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
    virtual bool equal(const value_ptr&) const override;

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

bool Boolean::equal(const value_ptr& other) const
{
    return other->type() == value_type::Boolean
           && val_ == other->as_bool();
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
    virtual bool equal(const value_ptr&) const override;

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

bool Integer::equal(const value_ptr& other) const
{
    return other->type() == value_type::Integer
           && val_ == other->as_int();
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
    virtual bool equal(const value_ptr&) const override;

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

bool String::equal(const value_ptr& other) const
{
    return other->type() == value_type::String
           && val_ == other->as_string();
}

std::ostream& String::display(std::ostream& o) const
{
    return o << val_;
}

/*
 * Symbol class
 */

class Symbol_value : public Value
{
public:
    Symbol_value(const Symbol& val) : val_(val) { }

    virtual const Symbol& as_symbol() const override;

    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(const value_ptr&) const override;

private:
    Symbol val_;
};

value_ptr mk_symbol(const Symbol& val)
{
    return value_ptr{new Symbol_value{val}};
}

const Symbol& Symbol_value::as_symbol() const
{
    return val_;
}

value_type Symbol_value::type() const
{
    return value_type::Symbol;
}

bool Symbol_value::equal(const value_ptr& other) const
{
    return other->type() == value_type::Symbol
           && val_ == other->as_symbol();
}

std::ostream& Symbol_value::display(std::ostream& o) const
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
    virtual bool equal(const value_ptr&) const override;

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

bool Cons::equal(const value_ptr& other) const
{
    return other->type() == value_type::Cons
           && first_->equal(other->first())
           && rest_->equal(other->rest());
}

/*
 * Empty class
 */

struct Empty : public Value
{
public:
    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(const value_ptr&) const override;
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

bool Empty::equal(const value_ptr& other) const
{
    return other->type() == value_type::Empty;
}

/*
 * Undefined class
 */

struct Undefined : public Value
{
public:
    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(const value_ptr&) const override;
};

value_ptr get_undefined()
{
    static value_ptr instance{new Undefined};
    return instance;
}

value_type Undefined::type() const
{
    return value_type::Undefined;
}

std::ostream& Undefined::display(std::ostream& o) const
{
    return o << "#<undefined>";
}

bool Undefined::equal(const value_ptr& other) const
{
    return other->type() == value_type::Undefined;
}

/*
 * Struct class
 */

class Struct : public Value
{
public:
    Struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
            : id_{id}, vals_(vals) { }

    virtual const struct_id_ptr         & struct_id() const override;
    virtual const std::vector<value_ptr>& get_fields() const override;

    virtual value_type  type() const override;
    virtual std::string type_name() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(const value_ptr&) const override;

private:
    struct_id_ptr          id_;
    std::vector<value_ptr> vals_;
};

value_ptr mk_struct(const struct_id_ptr& id, std::vector<value_ptr> vals)
{
    return value_ptr{new Struct{id, vals}};
}

const struct_id_ptr& Struct::struct_id() const
{
    return id_;
}

const std::vector<value_ptr>& Struct::get_fields() const
{
    return vals_;
}

value_type Struct::type() const
{
    return value_type::Struct;
}

std::string Struct::type_name() const
{
    return "struct " + id_->name.name();
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

bool Struct::equal(const value_ptr& other) const
{
    if (other->type() != value_type::Struct) return false;
    if (id_ != other->struct_id()) return false;
    if (vals_.size() != other->get_fields().size()) return false;

    for (auto i = vals_.begin(), j = other->get_fields().begin();
         i != vals_.end();
         ++i, ++j) {
        if (!(*i)->equal(*j)) return false;
    }

    return true;
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

bool Function::equal(const value_ptr&) const
{
    return false;
}

Function::Function(const Symbol& name, ssize_t arity)
        : arity_{arity}, name_{name} { }

}


