#include "symbol.h"

namespace islpp {

Symbol Symbol::uninterned(const std::string& name)
{
    return Symbol{name};
}

Symbol::Symbol(const std::string& name)
        : ptr_{std::make_shared<std::string>(name)}
{ }

std::ostream& operator<<(std::ostream& o, const Symbol& sym)
{
    return o << sym.name();
}

Symbol Intern_table::intern(const std::string& name)
{
    auto iter = table_.find(name);

    if (iter != table_.end()) {
        return iter->second;
    }

    Symbol sym = Symbol::uninterned(name);
    table_.insert({name, sym});

    return sym;
}

Intern_table& Intern_table::INSTANCE()
{
    static Intern_table instance;
    return instance;
}

}

