#pragma once

#include "symbol.h"

#include <memory>
#include <string>
#include <vector>

namespace islpp
{

struct Struct_id
{
    Struct_id(const Symbol& n, const std::vector<Symbol>& f)
            : name(n), fields(f) { }

    Symbol name;
    std::vector<Symbol> fields;
};

using struct_id_ptr = std::shared_ptr<Struct_id>;

}
