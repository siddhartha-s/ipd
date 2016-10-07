#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace islpp {

class Symbol {
public:
    bool operator==(const Symbol& other) const
    {
        return ptr_ == other.ptr_;
    }

    const std::string& name() const
    {
        return *ptr_;
    }

    static Symbol uninterned(const std::string&);

private:
    Symbol(const std::string&);

    std::shared_ptr<std::string> ptr_;
};

Symbol intern(const std::string&);

inline bool operator!=(const Symbol& a, const Symbol& b)
{
    return !(a == b);
}

std::ostream& operator<<(std::ostream&, const Symbol&);

class Intern_table
{
public:
    Symbol intern(const std::string&);

    static Intern_table& INSTANCE();

private:
    std::unordered_map<std::string, Symbol> table_;
};

}
